#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 lambdaPhi;

layout(binding = 0) uniform UBO {
    vec4 blackHole; // Position and Schwarzschild radius
    vec4 camera; // Position and zoom
    mat4 view; // Camera view (including aspect ratio)
    bool curve; // If true: Calculate curvature effects
    bool sun; // If true: Draw the sun
    float texScale; // For up/downscaling the texture
} ubo;

layout(binding = 1) uniform sampler2D tex;
layout(location = 0) out vec4 outColor;

mat3 rotationMatrix(vec3 axis, float angle) {
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat3(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c);
}

// TODO write a separate (maybe more generalizable) shader for rendering the sun
float RADIUS_OF_SUN = 6.96342e8;
vec3 sun_red    = vec3(0.93, 0.01, 0.02);
vec3 sun_orange = vec3(0.99, 0.34, 0.01);
vec3 sun_yellow = vec3(0.97, 0.67, 0.40);

void drawSun(float d) {
    vec3 color;
    if(d > 1) {
        color = 50000000 * (d-1) * pow(d-1.1, 6) * sun_red;
        if(d < 1.01) {
            color += 100 * (1.01-d) * sun_orange;
        }
    } else {
        color = d * sun_orange + (1-d) * sun_yellow;
    }
    outColor = vec4(color, 0);
}

void drawSunBeams(float d, float x, float y) {
    // Use doubling formulars to efficiently calculate the values for sin und cos
    float cos2alpha = x*x - y*y;
    float sin2alpha = 2*x*y;
    float cos4alpha = cos2alpha*cos2alpha - sin2alpha*sin2alpha;
    float sin4alpha = 2*cos2alpha*sin2alpha;
    float cos8alpha = cos4alpha*cos4alpha - sin4alpha*sin4alpha;
    float sin8alpha = 2*cos4alpha*sin4alpha;
    // Play around with this formula to achieve different sun beam effects
    outColor += (ubo.camera.w + 0.5 * sin8alpha * sin8alpha + 0.2 * cos8alpha + 0.1 * y) / ((1 + d * sqrt(d)) * ubo.camera.w) * vec4(1,1,1,0);
}

// This function is used for the approximation of the light distortion
float f(float phi, float R, float schwarzschildRadius) {
    return sin(phi) / R + schwarzschildRadius * (3 + cos(2 * phi)) / (4 * R * R);
}

float f_(float phi, float R, float schwarzschildRadius) {
    return cos(phi) / R - schwarzschildRadius * sin(2 * phi) / (2 * R * R);
}

void main() {
    vec3 blackHolePosition = ubo.blackHole.xyz;
    vec3 cameraPosition = ubo.camera.xyz;
    vec3 cameraToBlackHole = blackHolePosition - cameraPosition;
    float distance = length(cameraToBlackHole);
    float zoom = ubo.camera.w;
    float schwarzschildRadius = ubo.blackHole.w;

    vec3 normalized = normalize(ubo.view * 
            vec4(lambdaPhi.x, lambdaPhi.y, zoom, 0.0)
    ).xyz;

    if(dot(normalized, cameraToBlackHole) > 0) {
        vec3 top = cross(cameraToBlackHole, normalized);
        float R = length(top);

        if(ubo.sun) {
            if(R < 1.1 * RADIUS_OF_SUN) {
                drawSun(R / RADIUS_OF_SUN);
            } else {
                outColor = vec4(0,0,0,0);
            }
            drawSunBeams(R / RADIUS_OF_SUN, dot(normalize(top), normalize(ubo.view[0].xyz)), dot(normalize(top), ubo.view[1].xyz));
        } else {
            outColor = vec4(0,0,0,0);
        }

        // If activated, calculate the gravitational effects
        if(ubo.curve) {
            // You cannot see within the photon orbit
            if(R < 1.5 * schwarzschildRadius) {
                outColor = vec4(0,0,0,1);
                return;
            }
            // Use newtons method to aproximate the zero
            float alpha = -schwarzschildRadius / R;
            alpha -= f(alpha, R, schwarzschildRadius) / f_(alpha, R, schwarzschildRadius);

            vec3 rotAxis = normalize(cross(cameraToBlackHole, normalized));
            normalized = rotationMatrix(rotAxis, -2 * alpha) * normalized;
        }
    } else {
        outColor = vec4(0,0,0,0);
    }

    // To simulate a higher resolution of the texture multiply the texCoord with
    // a (large) factor.
    vec2 texCoord = ubo.texScale * vec2(
            -atan(normalized.x, normalized.y) / 2,
            acos(normalized.z)
    );
    outColor += 2 * texture(tex, texCoord / radians(180));
}

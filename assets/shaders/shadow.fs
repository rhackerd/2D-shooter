#version 330

uniform vec2 u_lightPos;
uniform float u_radius;
uniform int u_boxCount;      // how many boxes are active
uniform vec4 u_boxes[16];    // max 16 boxes

out vec4 fragColor;

void main() {
    vec2 fragPos = gl_FragCoord.xy;

    // base light intensity
    float dist = distance(fragPos, u_lightPos);
    float intensity = clamp(1.0 - dist/u_radius, 0.0, 1.0);

    // shadow check against all active boxes
    for (int i = 0; i < u_boxCount; i++) {
        if (fragPos.x > u_boxes[i].x && fragPos.x < u_boxes[i].x + u_boxes[i].z &&
            fragPos.y > u_boxes[i].y && fragPos.y < u_boxes[i].y + u_boxes[i].w) {
            intensity = 0.0;
        }
    }

    fragColor = vec4(vec3(intensity), 1.0);
}

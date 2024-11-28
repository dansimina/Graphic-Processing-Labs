#version 410 core

out float fragDepth;  // Output the depth as a float

void main()
{
    // The depth should be computed in the light's view space
    fragDepth = gl_FragCoord.z;  // Use the built-in gl_FragCoord for depth
}

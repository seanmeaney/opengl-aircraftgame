// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;
uniform float num_tiles;
uniform bool fill;
uniform bool shadow;

// Texture sampler
uniform sampler2D onetex;

void main()
{
    // Sample texture
    vec4 color;
    if(!fill){
        color = texture2D(onetex, vec2(uv_interp.x*num_tiles, uv_interp.y*num_tiles));
    } else{
        color = texture2D(onetex, vec2(uv_interp.x, uv_interp.y));
    }
    // Assign color to fragment
    if (!shadow){
        gl_FragColor = vec4(color.r, color.g, color.b, color.a);
    } else{
        gl_FragColor = vec4(0.3, 0.3, 0.3, 0.3);
    }

    // Check for transparency
    if(color.a < 1.0)
    {
        discard;
    }
}

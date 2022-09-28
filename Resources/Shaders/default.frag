/* default.frag file description:
* Defines the OpenGL code for the default fragment shader to be used in rendering.
* (The fragment shader here takes in information relating to rendering stuff and processes the final image of pixels.)
* (Also see the shader class for more use.)
*/

#version 330 core //(Helps OpenGL with version and related info.)

// Outputs colors in RGBA
out vec4 FragColor;

// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;
in vec3 color;

// Gets the Texture Unit from the main function
uniform sampler2D tex0;



void main()
{
	// Jankily discards transparent pixels (doesn't draw them) if the alpha channel is at least halfway.
	if(texture(tex0, texCoord).a < 0.5) {
		discard;
	}

	FragColor = texture(tex0, texCoord) * vec4(color, 1.0f);
}

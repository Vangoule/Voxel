#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;


void main(){


	//vec3 LightColor = vec3(0.9,0.1,0.1);
	vec3 LightColor = vec3(1.0,1.0,1.0);
	float LightPower = 50000.0f;
	

	vec3 MaterialDiffuseColor = texture2D( myTextureSampler, UV ).rgb;
	vec3 MaterialAmbiantColor = vec3(0.5,0.5,0.5) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.1,0.1,0.1);
	float distance = length( LightPosition_worldspace - Position_worldspace );


	vec3 n = normalize( Normal_cameraspace );

	vec3 l = normalize( LightDirection_cameraspace );

	float cosTheta = clamp( dot( n,l ), 0,1 );
	

	vec3 E = normalize(EyeDirection_cameraspace);

	vec3 R = reflect(-l,n);

	float cosAlpha = clamp( dot( E,R ), 0,1 );
	

	color = 
		// Ambiant : simulates indirect lighting
		MaterialAmbiantColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
	

}
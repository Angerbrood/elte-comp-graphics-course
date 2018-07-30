#version 130

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_normal;
in vec2 vs_out_tex0;

// kimenõ érték - a fragment színe
out vec4 fs_out_col;

//
// uniform változók
//
uniform vec3 eyePos;

// színtér tulajdonságok
vec3 lightDir = normalize(vec3(1, -1, 1));

// fénytulajdonságok
vec4 La = vec4(0.2f, 0.2f, 0.2f, 1);
vec4 Ld = vec4(0.8f, 0.8f, 0.8f, 1);
vec4 Ls = vec4(1, 1, 1, 1);

// anyagtulajdonságok
vec4 Ka = vec4(1, 1, 1, 1);
vec4 Kd = vec4(1, 1, 1, 1);
vec4 Ks = vec4(1, 1, 1, 1);

uniform sampler2D texture;
uniform sampler2D texture_2;

uniform float t;

void main()
{
	//
	// ambiens szín számítása
	//
	vec4 ambient = La * Ka;

	//
	// diffúz szín számítása
	//
	vec3 n = normalize(vs_out_normal);
	vec3 toLight = -lightDir;
	float di = clamp(dot(n, toLight), 0, 1);
	vec4 diffuse = Ld * Kd * di;

	/* segítség:
		- normalizálás: http://www.opengl.org/sdk/docs/manglsl/xhtml/normalize.xml
	    - skaláris szorzat: http://www.opengl.org/sdk/docs/manglsl/xhtml/dot.xml
	    - clamp: http://www.opengl.org/sdk/docs/manglsl/xhtml/clamp.xml
	*/
	
	//
	// fényfoltképzõ szín
	//
	vec3 r = reflect(lightDir, n);
	vec3 c = normalize(eyePos - vs_out_pos);
	float si = pow(clamp(dot(r, c), 0, 1), 16);
	vec4 specular = Ls * Ks * si;

	/* segítség:
		- reflect: http://www.opengl.org/sdk/docs/manglsl/xhtml/reflect.xml
		- power: http://www.opengl.org/sdk/docs/manglsl/xhtml/pow.xml
	*/
	
	//
	// a fragment végsõ színének meghatározása
	//
	fs_out_col = (1-t) * texture2D(texture, vs_out_tex0.st) + (t) * texture2D(texture_2, vs_out_tex0.st);// * (ambient + diffuse + specular);
}
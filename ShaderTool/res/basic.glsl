/**
 * https://www.shadertoy.com/view/XsXXDn
 * Credit to Danilo Guanabara
 */
#version 420 core

layout (location = 0) out vec4 oFragColor;

in vec2 aPos;
in vec2 aTexCoord;

uniform float uTime;
uniform vec2 uResolution;
uniform vec2 uMousePos;

void mainImage(out vec4 fragColor, in vec2 fragCoord){
	vec3 c;
	float l, z = uTime;
	for(int i = 0; i < 3; i++) {
		vec2 uv, p = fragCoord.xy / uResolution.xy;
		uv = p;
		p -= 0.5;
		p.x *= uResolution.x / uResolution.y;
		z += 0.07;
		l = length(p);
		uv += p / l * (sin(z) + 1.0) * abs(sin(l * 9.0 - z - z));
		c[i] = 0.01 / length(mod(uv, 1.0) - 0.5);
	}
	fragColor = vec4(c / l, uTime);
}

void main() {
	mainImage(oFragColor, gl_FragCoord.xy);
}

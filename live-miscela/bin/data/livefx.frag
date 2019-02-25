

#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359
#define TWO_PI 6.28318530718

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

uniform sampler2D u_tex0;
uniform sampler2D u_tex1;

uniform float u_var0;
uniform float u_var1;

float usin( float _time ){
    return (sin(_time) * 0.5)+0.5;
}

varying vec2 st;

void main(){
  //vec2 st = gl_FragCoord.xy/u_resolution.xy;
  vec4 source = texture2D( u_tex0, st ); // for texture access

  vec3 color = vec3 (1.0, usin( u_time*0.1)*u_var0, 0.0 );
  //vec3 color = vec3 (1.0);
  gl_FragColor = vec4( source.r, source.b*0.2, 0.0, source.a );
  //gl_FragColor = source;
}

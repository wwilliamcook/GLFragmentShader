#version 330 core

// uniforms
uniform vec2 viewportSize;
uniform vec2 mousePos;
uniform float time;

// outputs
out vec4 FragColor;

void main()
{
   float x_diff, y_diff, ref;
   ref = 1.0f;
   if (viewportSize.x > 0.0f)
      ref = viewportSize.x;
   if (viewportSize.y > 0.0f && viewportSize.x > viewportSize.y)
      ref = viewportSize.y;
   x_diff = ((gl_FragCoord.x - mousePos.x) / ref);
   y_diff = ((gl_FragCoord.y - (viewportSize.y - mousePos.y)) / ref);
   float distance = pow(x_diff*x_diff + y_diff*y_diff, 0.8f);
   FragColor = vec4(
      (1.0f - distance * 12.0f) * (cos(time * 0.5f + 3.14159f * 0.0f) * 0.5f + 0.5f),
      (1.0f - distance * 12.0f) * (cos(time * 0.5f + 3.14159f * 0.33333f) * 0.5f + 0.5f),
      (1.0f - distance * 12.0f) * (cos(time * 0.5f + 3.14159f * 0.66666f) * 0.5f + 0.5f),
      1.0f);
//   FragColor = vec4(1.0f - FragColor.x, 1.0f - FragColor.y, 1.0f - FragColor.z, 1.0f);  // invert colors
}

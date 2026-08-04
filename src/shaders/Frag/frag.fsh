#version 330 core

out vec4 FragColor;

uniform vec2 uCenters[6];
uniform float uRadius[6];

void main()
{
    // 화면 좌표 -> 네가 쓰던 좌표계
    vec2 p;

    p.x = gl_FragCoord.x - 800;
    p.y = 450 - gl_FragCoord.y;
    
    for(int i = 0; i < 6; i++)
    {
        float dx = uCenters[i].x - p.x;
        float dy = uCenters[i].y - p.y;
        
        if(dx*dx + dy*dy - uRadius[i]*uRadius[i] <= 1.0f)
        {
            FragColor = vec4(1,1,1,1);
            return;
        }
        else
        {
            FragColor = vec4(0,0,0,0);
        }
    }
}
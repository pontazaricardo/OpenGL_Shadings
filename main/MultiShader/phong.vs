uniform vec4 lightPos[4];
varying vec3 normal,eyeVec;
varying vec3 lightDir1,lightDir2,lightDir3,lightDir4;


void main()
	{	
	normal=gl_NormalMatrix*gl_Normal;

	vec3 vVertex=vec3(gl_ModelViewMatrix*gl_Vertex);

	//lightDir=vec3(gl_LightSource[0].position.xyz-vVertex);
	//lightDir=normalize(vec3(gl_LightSource[0].position)-vVertex);
	
	//for(int i=0;i<8;i++)
	//	lightDir[i]=lightPos[i].xyz-vVertex;
	
	lightDir1=lightPos[0].xyz-vVertex;
	lightDir2=lightPos[1].xyz-vVertex;
	lightDir3=lightPos[2].xyz-vVertex;
	lightDir4=lightPos[3].xyz-vVertex;

	
	
	
	eyeVec=-vVertex;
	gl_FrontColor=gl_Color;
	gl_Position=ftransform();		
	}
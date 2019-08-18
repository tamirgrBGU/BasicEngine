#define GLEW_STATIC
#include <GL\glew.h>
#include "scene.h"
#include <iostream>



	static void printMat(const glm::mat4 mat)
	{
		printf(" matrix: \n");
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				printf("%f ", mat[j][i]);
			printf("\n");
		}
	}

	Scene::Scene()
	{
		//verticesSize = sizeof(vertices)/sizeof(vertices[0]);
		//
		//indicesSize = sizeof(indices)/sizeof(indices[0]) ; 
		glLineWidth(5);
	
		cameras.push_back(new Camera(glm::vec3(0,0,1.0f),glm::vec3(0,0,-1.0f),60.0f,0.1f,100.0f));		
		pickedShape = -1;
		depth = 0;
		cameraIndx = 0;
		xold = 0;
		yold = 0;
		isActive = false;
	}

	Scene::Scene(glm::vec3 position,float angle,float near, float far)
	{
		//verticesSize = sizeof(vertices)/sizeof(vertices[0]);
		//
		//indicesSize = sizeof(indices)/sizeof(indices[0]) ; 
		glLineWidth(5);
		cameras.push_back(new Camera(position,-position,angle,near,far));
		pickedShape = -1;
		depth = 0;
		cameraIndx = 0;
		xold = 0;
		yold = 0;
		isActive = false;
	}

	void Scene::AddShapeFromFile(const std::string& fileName,int parent,unsigned int mode)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(fileName,mode));
	}

	void Scene::AddShape(int type, int parent,unsigned int mode)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(type,mode));
	}

	void Scene::AddShapeCopy(int indx,int parent,unsigned int mode)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(*shapes[indx],mode));
	}

	void Scene::AddShader(const std::string& fileName)
	{
		shaders.push_back(new Shader(fileName));
	}

	void Scene::AddTexture(const std::string& textureFileName,bool for2D)
	{
		textures.push_back(new Texture(textureFileName));
	}

	void Scene::AddTexture(int width,int height,int mode)
	{
		textures.push_back(new Texture(width,height,mode));
	}

	void Scene::AddCamera(glm::vec3& pos , float fov, float zNear, float zFar)
	{
		cameras.push_back(new Camera(pos,-pos,fov,zNear,zFar));
	}

	void Scene::Draw(int shaderIndx,int cameraIndx,int buffer,bool toClear,bool debugMode)
	{
		glEnable(GL_DEPTH_TEST);
		glm::mat4 Normal = MakeTrans();
	
		glm::mat4 MVP = cameras[cameraIndx]->GetViewProjection()*glm::inverse(cameras[cameraIndx]->MakeTrans());
		int p = pickedShape;
		if(toClear)
		{
			if(shaderIndx>0)
				Clear(1,1,1,1);
			else
				Clear(0,0,0,0);
		}

		for (unsigned int i=0; i<shapes.size();i++)
		{
			if(shapes[i]->Is2Render())
			{
				glm::mat4 Model = Normal * shapes[i]->MakeTrans();
				
				if(shaderIndx > 0)
				{
					Update(MVP,Model,shapes[i]->GetShader());
					shapes[i]->Draw(shaders,textures,false);	
				}
				else 
				{ //picking
					Update(MVP,Model,0);
					shapes[i]->Draw(shaders,textures,true);
				}
			}
		}
		pickedShape = p;
	}

	void Scene::MoveCamera(int cameraIndx,int type,float amt)
	{
		switch (type)
		{
		case FORWARD:
			cameras[cameraIndx]->MoveForward(amt);
			break;
		case UP:
			cameras[cameraIndx]->MoveUp(amt);
			break;
		case RIGHT:
			cameras[cameraIndx]->MoveRight(amt);
			break;
		case ROTLEFT:
			cameras[cameraIndx]->RotateY(amt);
			break;
		case ROTUP:
			cameras[cameraIndx]->Pitch(amt);
		break;
		default:
			break;
		}
	}

	void Scene::ShapeTransformation(int type,float amt)
	{
		//vec3 newAxis;
		if(glm::abs(amt)>1e-5)
		{
			switch (type)
			{
			case xTranslate:
				shapes[pickedShape]->MyTranslate(glm::vec3(amt,0,0),0);
			break;
			case yTranslate:
				shapes[pickedShape]->MyTranslate(glm::vec3(0,amt,0),0);
			break;
			case zTranslate:
				shapes[pickedShape]->MyTranslate(glm::vec3(0,0,amt),0);
			break;
			case xRotate:
				shapes[pickedShape]->MyRotate(amt,glm::vec3(1,0,0),0);
			break;
			case yRotate:
				shapes[pickedShape]->MyRotate(amt,glm::vec3(0,1,0),0);
			break;
			case zRotate:
				shapes[pickedShape]->MyRotate(amt,glm::vec3(0,0,1),0);
			break;
			default:
				break;
			}
		}

	}

	void Scene::Resize(int width,int height)
	{
		//glViewport(cameras[0]->GetLeft(),cameras[0]->GetBottom(),width,height);
		
		//cameras[0]->setProjection(cameras[cameraIndx]->GetNear(),cameras[cameraIndx]->GetFar(),Viewport(cameras[0]->GetLeft(),cameras[0]->GetBottom(),width,height));
		
	}

	float Scene::Picking(int x,int y)
	{
		
		
		return 0;
	}
	//return coordinates in global system for a tip of arm position is local system 
	void Scene::MouseProccessing(int button)
	{
		if(pickedShape == -1)
		{
			if(button == 1 )
			{				

				MyTranslate(glm::vec3(0,0,-xrel/10.0f),0);
				MyTranslate(glm::vec3(0,yrel/10.0f,0),0);
				WhenTranslate();
			}
			else
			{
				MyRotate(xrel/2.0f,glm::vec3(1,0,0),0);
				MyRotate(yrel/2.0f,glm::vec3(0,0,1),0);
				WhenRotate();
			}
		}
	}

	void Scene::ZeroShapesTrans()
	{
		for (unsigned int i = 0; i < shapes.size(); i++)
		{
			shapes[i]->ZeroTrans();
		}
	}

	void Scene::ReadPixel()
	{
		glReadPixels(1,1,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&depth);
	}

	void Scene::UpdatePosition(float xpos, float ypos)
	{
		xrel = xold - xpos;
		yrel = yold - ypos;
		xold = xpos;
		yold = ypos;
	}

	void Scene::HideShape(int shpIndx)
	{
			shapes[shpIndx]->Hide();
	}

	void Scene::UnhideShape(int shpIndx)
	{
			
			shapes[shpIndx]->Unhide();
	}

	void Scene::Clear(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	Scene::~Scene(void)
{
	for (Shape* shp : shapes)
		{
			delete shp;
		}
	for (Camera* cam : cameras)
		{
			delete cam;
		}
	for(Shader* sdr: shaders)
		{
			delete sdr;
		}
	for(Texture* tex: textures)
		{
			delete tex;
		}

}


	 
	
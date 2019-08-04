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
		glLineWidth(3);
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
		glLineWidth(6);
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
		//glEnable(GL_CULL_FACE);
		
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
				glm::mat4 Normal1 = glm::mat4(1);
				pickedShape = i;
				for (int j = i; chainParents[j] > -1; j = chainParents[j])
				{
					Normal1 =  shapes[chainParents[j]]->MakeTrans() * Normal1;
				}
			
				//mat4 MVP1 = MVP * Normal1; 
				Normal1 = Normal * Normal1;

				//MVP1 = MVP1 * shapes[i]->MakeTransScale(mat4(1));
				Normal1 = Normal1 * shapes[i]->MakeTrans();
				
				if(shaderIndx > 0)
				{
					Update(MVP,Normal1,shapes[i]->GetShader());
					shapes[i]->Draw(shaders,textures,false);
					
				}
				else 
				{ //picking
					Update(MVP,Normal1,0);
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
		//if(glm::abs(amt)>1e-5)
		//{
		//	switch (type)
		//	{
		//	case xLocalTranslate:
		//		if(pickedShape ==-1)
		//			myTranslate(vec3(amt,0,0),1);
		//		else
		//		{
		//			int i = pickedShape;
		//			for (; chainParents[i] > -1; i = chainParents[i]);
		//			shapes[i]->myTranslate(vec3(amt,0,0),1);
		//		}
		//		break;
		//	case yLocalTranslate:
		//		if(pickedShape ==-1)
		//			myTranslate(vec3(0,amt,0),1);
		//		else
		//			{
		//				int i = pickedShape;
		//				for (; chainParents[i] > -1; i = chainParents[i]);
		//				shapes[i]->myTranslate(vec3(0,amt,0),1);
		//			}
		//		break;
		//	case zLocalTranslate:
		//		if(pickedShape ==-1)
		//			myTranslate(vec3(0,0,amt),1);
		//		else
		//		{
		//			int i = pickedShape;
		//			for (; chainParents[i] > -1; i = chainParents[i]);
		//			shapes[i]->myTranslate(vec3(0,0,amt),1);
		//		}
		//		break;
		//	case xGlobalTranslate:
		//		if(pickedShape ==-1)
		//			myTranslate(vec3(amt/5.0,0,0),0);
		//		else
		//		{
		//			int i = pickedShape;
		//			for (; chainParents[i] > -1; i = chainParents[i]);
		//			shapes[i]->myTranslate(vec3(amt,0,0),0);
		//		}
		//		break;
		//	case yGlobalTranslate:
		//		if(pickedShape ==-1)
		//			myTranslate(vec3(0,amt/5.0,0),0);
		//		else
		//		{
		//			int i = pickedShape;
		//			for (; chainParents[i] > -1; i = chainParents[i]);
		//			shapes[i]->myTranslate(vec3(0,amt,0),0);
		//		}
		//		break;
		//	case zGlobalTranslate:
		//		if(pickedShape ==-1)
		//			myTranslate(vec3(0,0,amt/5.0),0);
		//						else
		//		{
		//			int i = pickedShape;
		//			for (; chainParents[i] > -1; i = chainParents[i]);
		//			shapes[i]->myTranslate(vec3(0,0,amt),0);
		//		}
		//		break;
		//	case xLocalRotate:
		//		if(pickedShape ==-1)
		//			myRotate(amt,vec3(1,0,0),xAxis1);
		//		else
		//			shapes[pickedShape]->myRotate(amt,vec3(1,0,0),xAxis1);
		//		break;
		//	case yLocalRotate:
		//		if(pickedShape ==-1)
		//			myRotate(amt,vec3(0,1,0),-1);
		//		else
		//			shapes[pickedShape]->myRotate(amt,vec3(0,1,0),-1);
		//		break;
		//	case zLocalRotate:
		//		if(pickedShape ==-1)
		//			myRotate(amt,vec3(0,0,1),zAxis1);
		//		else

		//			shapes[pickedShape]->myRotate(amt,vec3(0,0,1),zAxis1);
		//	break;
		//	case xGlobalRotate:
		//		if(pickedShape ==-1)
		//			globalSystemRot(amt,vec3(1,0,0),xAxis1);
		//		else
		//		{
		//			
		//			if(direction == -1 && pickedShape+2<shapes.size())
		//			{
		//				OpositeDirectionRot(glm::vec3(1,0,0),amt);
		//			}
		//			else
		//				shapes[pickedShape]->globalSystemRot(amt,vec3(1,0,0),xAxis1);
		//		}
		//		break;
		//	case yGlobalRotate:
		//		if(pickedShape ==-1)
		//			globalSystemRot(amt,vec3(0,1,0),-1);
		//		else
		//		{
		//			shapes[pickedShape]->globalSystemRot(amt,vec3(0,1,0),-1);
		//			if(direction == -1)
		//			{
		//				OpositeDirectionRot(glm::vec3(0,1,0),amt);
		//			}
		//			else
		//				shapes[pickedShape]->globalSystemRot(amt,vec3(1,0,0),xAxis1);
		//		}
		//		break;
		//	case zGlobalRotate:
		//		if(pickedShape ==-1)
		//			globalSystemRot(amt,vec3(0,0,1),zAxis12);
		//		else
		//		{
		//			
		//			
		//			if(direction == -1 && pickedShape+2<shapes.size())
		//			{
		//				OpositeDirectionRot(glm::vec3(0,0,1),amt);
		//			}
		//			else
		//				shapes[pickedShape]->globalSystemRot(amt,vec3(0,0,1),zAxis12);
		//		}
		//	break;
		//	case xScale:
		//		if(pickedShape ==-1)
		//			myScale(vec3(amt,1,1));
		//		else
		//			shapes[pickedShape]->myScale(vec3(amt,1,1));
		//	break;
		//	case yScale:
		//		if(pickedShape ==-1)
		//			myScale(vec3(1,amt,1));
		//		else
		//			shapes[pickedShape]->myScale(vec3(1,amt,1));
		//	break;
		//	case zScale:
		//		if(pickedShape ==-1)
		//			myScale(vec3(1,1,amt));
		//		else
		//			shapes[pickedShape]->myScale(vec3(1,1,amt));
		//	break;
		//	case xCameraTranslate: //camera plane translate
		//		if(pickedShape == -1)
		//			myTranslate(vec3(amt/5.0,0,0),0);
		//		else
		//		{
		//			//newAxis = findAxis(vec3(1,0,0));					
		//				int i = pickedShape;
		//			for (; chainParents[i] > 0; i = chainParents[i]);
		//		
		//			shapes[i]->translateInSystem(*this,vec3(amt,0,0),0,false);
		//		}
		//		break;
		//	case yCameraTranslate:
		//		if(pickedShape ==-1)
		//			myTranslate(vec3(0,amt/5.0,0),0);
		//		else
		//			{
		//				//newAxis = findAxis(vec3(0,1,0));
		//					int i = pickedShape;
		//				for (; chainParents[i] > 0; i = chainParents[i]);
		//				
		//				shapes[i]->translateInSystem(*this,vec3(0,amt,0),0,false);
		//			}
		//	break;
		//	case zCameraTranslate:
		//		if(pickedShape ==-1)
		//			myTranslate(vec3(0,0,amt/5.0),0);
		//		else
		//			{
		//			//	newAxis = findAxis(vec3(0,0,1));
		//					int i = pickedShape;
		//				for (; chainParents[i] > 0; i = chainParents[i]);
		//	
		//				shapes[i]->translateInSystem(*this,vec3(0,0,amt),0,false);
		//			}
		//	break;
		//	default:
		//		break;
		//	}
		//}

	}

	void Scene::Resize(int width,int height)
	{
		//glViewport(cameras[0]->GetLeft(),cameras[0]->GetBottom(),width,height);
		
		//cameras[0]->setProjection(cameras[cameraIndx]->GetNear(),cameras[cameraIndx]->GetFar(),Viewport(cameras[0]->GetLeft(),cameras[0]->GetBottom(),width,height));
		
	}

	float Scene::Picking(int x,int y)
	{
		
		Draw(0,0,BACK,true,false); 
		
		GLint viewport[4];  
		unsigned char data[4];
		glGetIntegerv(GL_VIEWPORT, viewport); //reading viewport parameters
		glReadPixels(x,viewport[3] - y,1,1, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glReadPixels(x,viewport[3] - y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&depth);
		std::cout<<"depth "<<depth<<std::endl;
		int pickedID = data[0] + data[1]*256 + data[2]*256*256 - 1;
		if(data[0] == 0 && data[1] == 0 && data[2] == 0)
		{
			pickedShape = -1;
			//std::cout<<"not picked"<<std::endl;
		}
		else
		{
			pickedShape = pickedID;
			std::cout<<"picked "<<pickedID<<std::endl;
			xold = x;
			yold = y;
		}
		return depth;
	}
	//return coordinates in global system for a tip of arm position is local system 
	void Scene::MouseProccessing(int button)
	{
		if(pickedShape == -1 || shapes[pickedShape]->Is2D())
		{
			if(button == 1 )
			{				

				ShapeTransformation(xCameraTranslate,-(float)xrel);
				ShapeTransformation(yCameraTranslate,(float)yrel);
				WhenTranslate();
			}
			else
			{
				ShapeTransformation(zGlobalRotate,(float)xrel*.5f);
				ShapeTransformation(xGlobalRotate,(float)yrel*.5f);
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

void Scene::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
	 
	
#include <sstream>
#include <fstream>
#include <string>

#include "raylib.h"
#include "rlgl.h"

#include "Geist/Globals.h"
#include "Geist/ResourceManager.h"
#include "Geist/Config.h"
#include "U7Globals.h"
#include "ShapeData.h"

using namespace std;

ShapeData::ShapeData()
{
	m_isValid = false;
	m_shape = 150;
	m_frame = 0;

   for (int i = 0; i < 6; ++i)
   {
		m_sideTexture[i] = CuboidTexture::CUBOID_INVALID;
	}
}

void ShapeData::Init(int shape, int frame, bool shouldreset)
{
	m_shape = shape;
	m_frame = frame;

   if(m_originalTexture != nullptr)
	{
		m_isValid = true;

      //  We need local copies of this texture for three sides of the cuboid.
      if (m_topTexture == nullptr)
      {
         m_topTexture = std::make_unique<ModTexture>(m_originalTexture->m_Image);
      }
      if (m_frontTexture == nullptr)
      {
         m_frontTexture = std::make_unique<ModTexture>(m_originalTexture->m_Image);
      }
      if (m_rightTexture == nullptr)
      {
         m_rightTexture = std::make_unique<ModTexture>(m_originalTexture->m_Image);
      }

      ObjectData* objectData = &g_objectTable[m_shape];

      if (shouldreset)
      {
			ResetTopTexture();
			ResetFrontTexture();
			ResetRightTexture();
		}

      for (int i = 0; i < 6; ++i)
      {
         if (m_sideTexture[i] == CuboidTexture::CUBOID_INVALID)
         {
            switch (static_cast<CuboidSides>(i))
            {
               case CuboidSides::CUBOID_TOP:
               case CuboidSides::CUBOID_BOTTOM:
						m_sideTexture[i] = CuboidTexture::CUBOID_DRAW_TOP;
						break;
               case CuboidSides::CUBOID_LEFT:
               case CuboidSides::CUBOID_RIGHT:
   					m_sideTexture[i] = CuboidTexture::CUBOID_DRAW_RIGHT;
						break;
               case CuboidSides::CUBOID_BACK:
               case CuboidSides::CUBOID_FRONT:
						m_sideTexture[i] = CuboidTexture::CUBOID_DRAW_FRONT;
						break;
            }
         }
      }

      //m_sideTexture[static_cast<int>(CuboidSides::CUBOID_TOP)] = CuboidTexture::CUBOID_DRAW_TOP;
      //m_sideTexture[static_cast<int>(CuboidSides::CUBOID_BOTTOM)] = CuboidTexture::CUBOID_DRAW_TOP;
      //m_sideTexture[static_cast<int>(CuboidSides::CUBOID_FRONT)] = CuboidTexture::CUBOID_DRAW_FRONT;
      //m_sideTexture[static_cast<int>(CuboidSides::CUBOID_BACK)] = CuboidTexture::CUBOID_DRAW_FRONT;
      //m_sideTexture[static_cast<int>(CuboidSides::CUBOID_LEFT)] = CuboidTexture::CUBOID_DRAW_RIGHT;
      //m_sideTexture[static_cast<int>(CuboidSides::CUBOID_RIGHT)] = CuboidTexture::CUBOID_DRAW_RIGHT;

      SetupDrawTypes();
	}
   else
   {
      m_isValid = false;
   }
}

void ShapeData::SetDefaultTexture(Image image)
{
   if (m_originalTexture == nullptr)
   {
		m_originalTexture = std::make_unique<ModTexture>(image);
	}
   else
   {
		m_originalTexture->AssignImage(image);
	}

}

void ShapeData::CreateDefaultTexture()
{
   if (m_originalTexture == nullptr)
   {
		m_originalTexture = std::make_unique<ModTexture>();
	}
}

void ShapeData::Serialize(ofstream& outStream)
{
   outStream << m_shape << " ";
   outStream << m_frame << " ";
   outStream << m_topTextureOffsetX << " ";
   outStream << m_topTextureOffsetY << " ";
   outStream << m_topTextureWidth << " ";
   outStream << m_topTextureHeight << " ";
   outStream << m_frontTextureOffsetX << " ";
   outStream << m_frontTextureOffsetY << " ";
   outStream << m_frontTextureWidth << " ";
   outStream << m_frontTextureHeight << " ";
   outStream << m_rightTextureOffsetX << " ";
   outStream << m_rightTextureOffsetY << " ";
   outStream << m_rightTextureWidth << " ";
   outStream << m_rightTextureHeight << " ";
   outStream << static_cast<int>(m_drawType) << " ";
   outStream << m_Scaling.x << " ";
   outStream << m_Scaling.y << " ";
   outStream << m_Scaling.z << " ";
   outStream << static_cast<int>(m_sideTexture[0]) << " ";
   outStream << static_cast<int>(m_sideTexture[1]) << " ";
   outStream << static_cast<int>(m_sideTexture[2]) << " ";
   outStream << static_cast<int>(m_sideTexture[3]) << " ";
   outStream << static_cast<int>(m_sideTexture[4]) << " ";
   outStream << static_cast<int>(m_sideTexture[5]) << " ";
   outStream << endl;

   outStream.flush();
}

void ShapeData::Deserialize(ifstream& inStream)
{
   inStream >> m_shape;
	inStream >> m_frame;
	inStream >> m_topTextureOffsetX;
	inStream >> m_topTextureOffsetY;
	inStream >> m_topTextureWidth;
	inStream >> m_topTextureHeight;
	inStream >> m_frontTextureOffsetX;
	inStream >> m_frontTextureOffsetY;
	inStream >> m_frontTextureWidth;
	inStream >> m_frontTextureHeight;
	inStream >> m_rightTextureOffsetX;
	inStream >> m_rightTextureOffsetY;
	inStream >> m_rightTextureWidth;
	inStream >> m_rightTextureHeight;
	int drawType;
	inStream >> drawType;
	m_drawType = static_cast<ShapeDrawType>(drawType);
	inStream >> m_Scaling.x;
	inStream >> m_Scaling.y;
	inStream >> m_Scaling.z;
	int sideTexture;
	inStream >> sideTexture;
	m_sideTexture[0] = static_cast<CuboidTexture>(sideTexture);
	inStream >> sideTexture;
	m_sideTexture[1] = static_cast<CuboidTexture>(sideTexture);
	inStream >> sideTexture;
	m_sideTexture[2] = static_cast<CuboidTexture>(sideTexture);
	inStream >> sideTexture;
	m_sideTexture[3] = static_cast<CuboidTexture>(sideTexture);
	inStream >> sideTexture;
	m_sideTexture[4] = static_cast<CuboidTexture>(sideTexture);
	inStream >> sideTexture;
	m_sideTexture[5] = static_cast<CuboidTexture>(sideTexture);

   Init(m_shape, m_frame, false);

}

void ShapeData::ResetTopTexture()
{
   ObjectData* objectData = &g_objectTable[m_shape];

   m_topTextureOffsetX = 0;
   m_topTextureOffsetY = 0;
   m_topTextureWidth = objectData->m_width * 8;
   m_topTextureHeight = objectData->m_depth * 8;

   SafeAndSane();
}

void ShapeData::ResetFrontTexture()
{
   ObjectData* objectData = &g_objectTable[m_shape];

   m_frontTextureOffsetX = 0;
   m_frontTextureOffsetY = objectData->m_depth * 8;
   m_frontTextureWidth = objectData->m_width * 8;
   m_frontTextureHeight = m_originalTexture->height - objectData->m_depth * 8;

   SafeAndSane();
}


void ShapeData::ResetRightTexture()
{
   ObjectData* objectData = &g_objectTable[m_shape];

   m_rightTextureOffsetX = objectData->m_width * 8;
   m_rightTextureOffsetY = 0;
   m_rightTextureWidth = m_originalTexture->width - objectData->m_width * 8;
   m_rightTextureHeight = objectData->m_depth * 8;

   SafeAndSane();
}


void ShapeData::SafeAndSane()
{
   if (m_topTextureWidth < 0) { m_topTextureWidth = 0; }
   if (m_topTextureHeight < 0) { m_topTextureHeight = 0; }
   if (m_frontTextureWidth < 0) { m_frontTextureWidth = 0; }
   if (m_frontTextureHeight < 0) { m_frontTextureHeight = 0; }
   if (m_rightTextureWidth < 0) { m_rightTextureWidth = 0; }
   if (m_rightTextureHeight < 0) { m_rightTextureHeight = 0; }

   if (m_topTextureWidth > m_originalTexture->width) { m_topTextureWidth = m_originalTexture->width; }
   if (m_topTextureHeight > m_originalTexture->height) { m_topTextureHeight = m_originalTexture->height; }
   if (m_frontTextureWidth > m_originalTexture->width) { m_frontTextureWidth = m_originalTexture->width; }
   if (m_frontTextureHeight > m_originalTexture->height) { m_frontTextureHeight = m_originalTexture->height; }
   if (m_rightTextureWidth > m_originalTexture->width) { m_rightTextureWidth = m_originalTexture->width; }
   if (m_rightTextureHeight > m_originalTexture->height) { m_rightTextureHeight = m_originalTexture->height; }

   if (m_topTextureOffsetX < 0) { m_topTextureOffsetX = 0; }
   if (m_topTextureOffsetY < 0) { m_topTextureOffsetY = 0; }
   if (m_frontTextureOffsetX < 0) { m_frontTextureOffsetX = 0; }
   if (m_frontTextureOffsetY < 0) { m_frontTextureOffsetY = 0; }
   if (m_rightTextureOffsetX < 0) { m_rightTextureOffsetX = 0; }
   if (m_rightTextureOffsetY < 0) { m_rightTextureOffsetY = 0; }

   if (m_topTextureOffsetX > m_originalTexture->width) { m_topTextureOffsetX = m_originalTexture->width; }
   if (m_topTextureOffsetY > m_originalTexture->height) { m_topTextureOffsetY = m_originalTexture->height; }
   if (m_frontTextureOffsetX > m_originalTexture->width) { m_frontTextureOffsetX = m_originalTexture->width; }
   if (m_frontTextureOffsetY > m_originalTexture->height) { m_frontTextureOffsetY = m_originalTexture->height; }
   if (m_rightTextureOffsetX > m_originalTexture->width) { m_rightTextureOffsetX = m_originalTexture->width; }
   if (m_rightTextureOffsetY > m_originalTexture->height) { m_rightTextureOffsetY = m_originalTexture->height; }


}

void ShapeData::SetupDrawTypes()
{
   ObjectData* objectData = &g_objectTable[m_shape];

   if (m_drawType == ShapeDrawType::OBJECT_DRAW_BILLBOARD)
   {
      m_Scaling = Vector3{ float(m_originalTexture->width) / 8.0f, float(m_originalTexture->height) / 8.0f, 1 };
   }
   else if (m_drawType == ShapeDrawType::OBJECT_DRAW_FLAT)
   {
      m_Scaling = Vector3{ float(m_originalTexture->width) / 8.0f, 0, float(m_originalTexture->height) / 8.0f };
   }
   else
   {
      m_Scaling = Vector3{ objectData->m_width, objectData->m_height, objectData->m_depth };
   }

   FixupTextures();

   //  BILLBOARD DRAWING
   Mesh billboardMesh = GenMeshPlane(m_Scaling.x, m_Scaling.y, 1, 1);

   //  Move the mesh from the center to the corner
   for (int i = 0; i < billboardMesh.vertexCount; ++i)
   {
		billboardMesh.vertices[i * 3];
      float y = billboardMesh.vertices[i * 3 + 2];
		billboardMesh.vertices[i * 3 + 1] = billboardMesh.vertices[i * 3 + 2];
      billboardMesh.vertices[i * 3 + 2] = y;
      float u = billboardMesh.texcoords[i * 2];
      billboardMesh.texcoords[i * 2] = billboardMesh.texcoords[i * 2 + 1];
      billboardMesh.texcoords[i * 2 + 1] = u;
	}

   UpdateMeshBuffer(billboardMesh, 0, billboardMesh.vertices, sizeof(float) * billboardMesh.vertexCount * 3, 0);
   m_billboardModel = LoadModelFromMesh(billboardMesh);
   SetMaterialTexture(&m_billboardModel.materials[0], MATERIAL_MAP_DIFFUSE, m_originalTexture->m_Texture);

   //  FLAT DRAWING

   Mesh flatMesh = GenMeshPlane(m_Scaling.x, m_Scaling.z, 1, 1);

   //  Move the mesh from the center to the corner
   for (int i = 0; i < flatMesh.vertexCount; ++i)
   {
      flatMesh.vertices[i * 3] -= ((m_Scaling.x / 2) - 1);
      flatMesh.vertices[i * 3 + 2] -= ((m_Scaling.z / 2) - 1);
   }

   UpdateMeshBuffer(flatMesh, 0, flatMesh.vertices, sizeof(float) * flatMesh.vertexCount * 3, 0);

   m_flatModel = LoadModelFromMesh(flatMesh);
   SetMaterialTexture(&m_flatModel.materials[0], MATERIAL_MAP_DIFFUSE, m_originalTexture->m_Texture);

   //  CUBOID DRAWING

   //  Very similar to flat drawing, we need to build six custom meshes to represent the sides.
   Mesh cuboidMesh = GenMeshCube(m_Scaling.x, m_Scaling.y, m_Scaling.z);

   //  Move the mesh from the center to the corner
   for (int i = 0; i < cuboidMesh.vertexCount; ++i)
   {
      cuboidMesh.vertices[i * 3] -= ((m_Scaling.x / 2) - 1);
		cuboidMesh.vertices[i * 3 + 1] += (m_Scaling.y / 2);
		cuboidMesh.vertices[i * 3 + 2] -= ((m_Scaling.z / 2) - 1);
	}

   UpdateMeshBuffer(cuboidMesh, 0, cuboidMesh.vertices, sizeof(float) * cuboidMesh.vertexCount * 3, 0);

   m_cuboidModels[static_cast<int>(CuboidSides::CUBOID_BOTTOM)] = LoadModelFromMesh(cuboidMesh);

   //m_cuboidModels[static_cast<int>(CuboidSides::CUBOID_BOTTOM)] = LoadModelFromMesh(flatMesh);
   m_cuboidModels[static_cast<int>(CuboidSides::CUBOID_BACK)] = LoadModelFromMesh(flatMesh);
   m_cuboidModels[static_cast<int>(CuboidSides::CUBOID_FRONT)] = LoadModelFromMesh(flatMesh);
   m_cuboidModels[static_cast<int>(CuboidSides::CUBOID_LEFT)] = LoadModelFromMesh(flatMesh);
   m_cuboidModels[static_cast<int>(CuboidSides::CUBOID_RIGHT)] = LoadModelFromMesh(flatMesh);
   m_cuboidModels[static_cast<int>(CuboidSides::CUBOID_TOP)] = LoadModelFromMesh(flatMesh);

   //  Set the textures for each of the six sides.

   SetMaterialTexture(&m_cuboidModels[static_cast<int>(CuboidSides::CUBOID_BOTTOM)].materials[0], MATERIAL_MAP_DIFFUSE, m_topTexture->m_Texture);
	SetMaterialTexture(&m_cuboidModels[static_cast<int>(CuboidSides::CUBOID_TOP)].materials[0], MATERIAL_MAP_DIFFUSE, m_topTexture->m_Texture);
   SetMaterialTexture(&m_cuboidModels[static_cast<int>(CuboidSides::CUBOID_FRONT)].materials[0], MATERIAL_MAP_DIFFUSE, m_frontTexture->m_Texture);
   SetMaterialTexture(&m_cuboidModels[static_cast<int>(CuboidSides::CUBOID_BACK)].materials[0], MATERIAL_MAP_DIFFUSE, m_frontTexture->m_Texture);
   SetMaterialTexture(&m_cuboidModels[static_cast<int>(CuboidSides::CUBOID_LEFT)].materials[0], MATERIAL_MAP_DIFFUSE, m_rightTexture->m_Texture);
   SetMaterialTexture(&m_cuboidModels[static_cast<int>(CuboidSides::CUBOID_RIGHT)].materials[0], MATERIAL_MAP_DIFFUSE, m_rightTexture->m_Texture);

};

void ShapeData::FixupTextures()
{
   //  Fixup the texture for this object.

   ObjectData* objectData = &g_objectTable[m_shape];

   //  Reset all textures
   m_topTexture->Reset();
   m_frontTexture->Reset();
   m_rightTexture->Reset();

   //  Top face
   //  Move pixels not part of this face off the texture
   for (int j = 0; j < m_topTextureOffsetY; ++j)
   {
      for (int i = 0; i < m_topTexture->width; ++i)
      {
         m_topTexture->MoveImageColumnUp(i);
      }
   }

   for (int j = 0; j < m_topTextureOffsetX; ++j)
   {
      for (int i = 0; i < m_topTexture->height; ++i) 
      {
         m_topTexture->MoveImageRowLeft(i);
      }
   }

   m_topTexture->ResizeImage(m_topTextureWidth, m_topTextureHeight);
   m_topTexture->UpdateTexture();

   //  Front face
   //  Move pixels not part of this face off the texture
   for (int j = 0; j < m_frontTextureOffsetY; ++j)
   {
      for (int i = 0; i < m_frontTexture->width; ++i)
      {
         m_frontTexture->MoveImageColumnUp(i);
      }
   }

   for (int j = 0; j < m_frontTextureOffsetX; ++j)
   {
      for (int i = 0; i < m_frontTexture->height; ++i)
      {
         m_frontTexture->MoveImageRowLeft(i);
      }
   }

   //  Shift slanted pixels to unslant
   int counter = 1;
   for (int i = 0; i < m_frontTexture->height; ++i)
   {
      for (int k = 0; k < counter; ++k)
      {
         m_frontTexture->MoveImageRowLeft(i);
      }
      ++counter;
   }

   m_frontTexture->ResizeImage(m_frontTextureWidth, m_frontTextureHeight);
   m_frontTexture->UpdateTexture();

   //  Right face
   //  Move pixels not part of this face off the texture
   for (int j = 0; j < m_rightTextureOffsetY; ++j)
   {
      for (int i = 0; i < m_rightTexture->width; ++i)
      {
         m_rightTexture->MoveImageColumnUp(i);
      }
   }

   for (int j = 0; j < m_rightTextureOffsetX; ++j)
   {
      for (int i = 0; i < m_rightTexture->height; ++i)
      {
         m_rightTexture->MoveImageRowLeft(i);
      }
   }

   //  Shift slanted pixels to unslant
   counter = 1;
   for (int i = 0; i < m_rightTextureWidth; ++i)
   {
      for(int k = 0; k < counter; ++k)
      {
         m_rightTexture->MoveImageColumnUp(i);
      }
      ++counter;
   }

   m_rightTexture->ResizeImage(m_rightTextureWidth, m_rightTextureHeight);
   m_rightTexture->UpdateTexture();

}

void ShapeData::Draw(const Vector3& pos, float angle, Color color)
{
   if (m_isValid == false)
   {
		return;
	}

   ObjectData* objectData = &g_objectTable[m_shape];

   switch (m_drawType)
   {
      case ShapeDrawType::OBJECT_DRAW_CUBOID:
      {
         Vector3 thisPos = pos;

         DrawModel(m_cuboidModels[static_cast<int>(CuboidSides::CUBOID_BOTTOM)], thisPos, 1, WHITE);
         //DrawModel(m_cuboidModels[static_cast<int>(CuboidSides::CUBOID_LEFT)], thisPos, 1, WHITE);
         //DrawModel(m_cuboidModels[static_cast<int>(CuboidSides::CUBOID_RIGHT)], thisPos, 1, WHITE);
         //DrawModel(m_cuboidModels[static_cast<int>(CuboidSides::CUBOID_FRONT)], thisPos, 1, WHITE);
         //DrawModel(m_cuboidModels[static_cast<int>(CuboidSides::CUBOID_BACK)], thisPos, 1, WHITE);
         //DrawModel(m_cuboidModels[static_cast<int>(CuboidSides::CUBOID_TOP)], thisPos, 1, WHITE);

         break;
      }

      case ShapeDrawType::OBJECT_DRAW_FLAT:
      {
         Vector3 thisPos = pos;
         if (pos.y == 0)
         {
				thisPos.y = .1f; //  Otherwise, z-fighting.
			}
         else
         {
            thisPos.y = pos.y * 1.1f;
         }
         DrawModel(m_flatModel, thisPos, 1, WHITE);
         break;
      }

      case ShapeDrawType::OBJECT_DRAW_BILLBOARD:
      case ShapeDrawType::OBJECT_DRAW_CUSTOM_MESH:
      {
         Vector3 thisPos = pos;
         thisPos.x += .5f;
         thisPos.z += .5f;
         thisPos.y += m_Scaling.y * .60f;

         //DrawModel(m_billboardModel, thisPos, 1, WHITE);
         
         DrawBillboardPro(g_camera, m_originalTexture->m_Texture, Rectangle{ 0, 0, float(m_originalTexture->m_Texture.width), float(m_originalTexture->m_Texture.height) }, thisPos, Vector3{ 0, 1, 0 },
            Vector2{ m_Scaling.x, m_Scaling.y }, Vector2{ 0, 0 }, -45, color);
         break;
      }
   }
}

bool ShapeData::Pick(Vector3 thisPos)
{
   bool picked = false;

   Ray ray = GetScreenToWorldRay(GetMousePosition(), g_camera);

   BoundingBox box = {
		Vector3{ thisPos.x - m_Scaling.x / 2, thisPos.y - m_Scaling.y / 2, thisPos.z - m_Scaling.z / 2 },
		Vector3{ thisPos.x + m_Scaling.x / 2, thisPos.y + m_Scaling.y / 2, thisPos.z + m_Scaling.z / 2 }
	};

   // Check collision between ray and box
   RayCollision collision = GetRayCollisionBox(ray, box);

   return collision.hit;
}
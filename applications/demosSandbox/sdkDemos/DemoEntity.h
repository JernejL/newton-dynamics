/* Copyright (c) <2003-2019> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/

#ifndef __DEMO_ENTITY_H__
#define __DEMO_ENTITY_H__

#include "DemoEntityManager.h"

class ShaderPrograms;
class DemoMeshInterface;


class DemoEntity: public dHierarchy<DemoEntity>, virtual public dClassInfo
{
	public:

	class UserData
	{
		public:
		UserData()
		{
		}

		virtual ~UserData()
		{
		}
		
		virtual void OnRender (dFloat timestep) const = 0;
	};

	DemoEntity(const DemoEntity& copyFrom);
	DemoEntity(const dMatrix& matrix, DemoEntity* const parent);
	DemoEntity(DemoEntityManager& world, const dScene* const scene, dScene::dTreeNode* const rootSceneNode, dTree<DemoMeshInterface*, dScene::dTreeNode*>& meshCache, DemoEntityManager::EntityDictionary& entityDictionary, DemoEntity* const parent = NULL);
	virtual ~DemoEntity(void);

	DemoMeshInterface* GetMesh() const;
	void SetMesh (DemoMeshInterface* const m_mesh, const dMatrix& meshMatrix);

	const dMatrix& GetMeshMatrix() const;  
	void SetMeshMatrix(const dMatrix& matrix);  

	UserData* GetUserData ();
	void SetUserData (UserData* const data);

	dBaseHierarchy* CreateClone () const;
	static DemoEntity* LoadNGD_mesh (const char* const fileName, NewtonWorld* const world, const ShaderPrograms& shaderCache);

	const dMatrix& GetRenderMatrix () const;
	dMatrix CalculateGlobalMatrix (const DemoEntity* const root = NULL) const;

	dMatrix GetNextMatrix () const;
	dMatrix GetCurrentMatrix () const;
	virtual void SetMatrix(DemoEntityManager& world, const dQuaternion& rotation, const dVector& position);
	virtual void SetNextMatrix (DemoEntityManager& world, const dQuaternion& rotation, const dVector& position);

	void InterpolateMatrixUnsafe(dFloat param);
	void SetMatrixUsafe(const dQuaternion& rotation, const dVector& position);

	virtual void ResetMatrix(DemoEntityManager& world, const dMatrix& matrix);
	virtual void InterpolateMatrix (DemoEntityManager& world, dFloat param);
	dMatrix CalculateInterpolatedGlobalMatrix (const DemoEntity* const root = NULL) const;

	void RenderBone() const;
	virtual void Render(dFloat timeStep, DemoEntityManager* const scene, const dMatrix& matrix) const;
	virtual void SimulationPreListener(DemoEntityManager* const scene, DemoEntityManager::dListNode* const mynode, dFloat timeStep){};
	virtual void SimulationPostListener(DemoEntityManager* const scene, DemoEntityManager::dListNode* const mynode, dFloat timeStep){};

	NewtonCollision* CreateCollisionFromchildren(NewtonWorld* const world) const;

	virtual void MessageHandler (NewtonBody* const sender, int message, void* const data) {}
	static void TransformCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex);

	protected:
	mutable dMatrix m_matrix;			// interpolated matrix
	dVector m_curPosition;				// position one physics simulation step in the future
	dVector m_nextPosition;             // position at the current physics simulation step
	dQuaternion m_curRotation;          // rotation one physics simulation step in the future  
	dQuaternion m_nextRotation;         // rotation at the current physics simulation step  

	dMatrix m_meshMatrix;
	DemoMeshInterface* m_mesh;
	UserData* m_userData;
	unsigned m_lock;
	bool m_isVisible;
	dAddRtti(dClassInfo,DOMMY_API);

	friend class DemoEntityManager;
};

#endif
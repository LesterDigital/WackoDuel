#include "enginepch.h"
#include "BulletScript.h"

#include "ShooterController.h"

#include "AssetManager/AssetTypes.h"

void WD::BulletScript::OnCreate()
{
	auto* script = (LeftShooterController*)GetEntityByName(m_MasterName)[0].GetComponent<NativeScriptComponent>().Instance;
	m_Direction = script->GetAcceleration();

	float length = std::sqrtf(m_Direction.x * m_Direction.x + m_Direction.y * m_Direction.y);

	//if (length == 0.0f)
		//script->GetDirection()
		
	m_Direction /= length;
	
	GetComponent<RenderComponent>().Texture = LoadAsset<Texture2DAsset>("/Assets/Textures/rock")->GetTexture();
}

void WD::BulletScript::OnUpdate(Timestep ts)
{
	auto& pos = GetComponent<TransformComponent>().Translation;
	if (GetEntityByName(m_EnemyName).size() > 0)
	{
		auto& enemyTC = GetEntityByName(m_EnemyName)[0].GetComponent<TransformComponent>();

		float leftX = enemyTC.Translation.x - 0.5f * enemyTC.Scale.x;
		float rightX = enemyTC.Translation.x + 0.5f * enemyTC.Scale.x;
		float bottomY = enemyTC.Translation.y - 0.5f * enemyTC.Scale.y;
		float topY = enemyTC.Translation.y + 0.5f * enemyTC.Scale.y;

		if (leftX <= pos.x && rightX >= pos.x && bottomY <= pos.y && topY >= pos.y)
		{
			DestroySelf();

			if (m_EnemyName == "Shooter2")
				((RightShooterController*)(GetEntityByName(m_EnemyName)[0].GetComponent<NativeScriptComponent>().Instance))->OnDamageRecieve(20);
			else if (m_EnemyName == "Shooter1")
				((LeftShooterController*)(GetEntityByName(m_EnemyName)[0].GetComponent<NativeScriptComponent>().Instance))->OnDamageRecieve(20);
			
			return;
		}
	}
	float border = 8.0f;

	if (pos.x >= border || pos.x <= -border || pos.y >= border || pos.y <= -border)
	{
		DestroySelf();
		return;
	}


	pos.x += m_Direction.x * m_Speed * ts;
	pos.y += m_Direction.y * m_Speed * ts;

	GetComponent<TransformComponent>().Rotation.z += glm::radians(m_Speed * 100) * ts;
}

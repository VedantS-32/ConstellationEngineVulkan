#pragma once

#include "Minimal.h"
#include "CStellWindow.h"
#include "CStellShaderPipeline.h"
#include "CStellDevice.h"
#include "CStellRenderer.h"
#include "CStellGameObject.h"

#include <memory>
#include <vector>

namespace CStell
{
	class CStellApplication
	{
	public:
		CStellApplication();
		~CStellApplication();

		CStellApplication(const CStellApplication&) = delete;
		CStellApplication& operator=(const CStellApplication&) = delete;

		static constexpr int m_Width = 800;
		static constexpr int m_Height = 600;

		void Run();

	private:
		void LoadGameObjects();

		CStellWindow m_CStellWindow{m_Width, m_Height, "Constellation Engine"};
		CStellDevice m_CStellDevice{m_CStellWindow};
		CStellRenderer m_CStellRenderer{m_CStellWindow, m_CStellDevice};

		std::vector<CStellGameObject> m_GameObjects;
	};
}
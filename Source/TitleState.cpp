#include "Globals.h"
#include "U7Globals.h"
#include "TitleState.h"

#include <list>
#include <string>
#include <sstream>
#include <math.h>
#include <fstream>
#include <algorithm>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//  TitleState
////////////////////////////////////////////////////////////////////////////////

TitleState::~TitleState()
{
   Shutdown();
}

void TitleState::Init(const string& configfile)
{
    MakeAnimationFrameMeshes();

   CreateTitleGUI();
}

void TitleState::OnEnter()
{
   ClearConsole();
}

void TitleState::OnExit()
{

}

void TitleState::Shutdown()
{

}

void TitleState::Update()
{
   UpdateTitle();
   //g_StateMachine->MakeStateTransition(STATE_MAINSTATE);
}


void TitleState::Draw()
{
   g_Display->ClearScreen();

   m_TitleGui->Draw();
   
   DrawConsole();
   
   g_Display->DrawImage(g_Cursor, g_Input->m_MouseX, g_Input->m_MouseY);

}

////////////////////////////////////////////////////////////////////////////////
///  CREATORS
////////////////////////////////////////////////////////////////////////////////

void TitleState::CreateTitleGUI()
{
   m_TitleGui = new Gui();
   m_TitleGui->SetLayout(0, 0, 320, 180, Gui::GUIP_CENTER);
   //m_TitleGui->AddPanel(GUI_TITLE_PANEL1, 0, 0, 138, 384, Color(0, 0, 0, .75));
   //m_TitleGui->AddPanel(GUI_TITLE_PANEL2, 0, 0, 320, 180, Color(1, 1, 1, 1), false);
   m_TitleGui->AddTextArea(GUI_TITLE_TITLE, g_Font.get(), "Ultima VII: Revisited", (g_Display->GetWidth() - (g_Font->GetStringMetrics("Ultima VII: Revisited"))) / 2, 20, (g_Font->GetStringMetrics("Ultima VII: Revisited")), 0, Color(1, 1, 1, 1), true);
   m_TitleGui->AddTextButton(GUI_TITLE_BUTTON_SINGLE_PLAYER, (320 - (g_SmallFont->GetStringMetrics("Begin") + 4)) / 2, 30, "Begin", g_SmallFont.get());
   m_TitleGui->AddTextButton(GUI_TITLE_BUTTON_OBJECT_EDITOR, (320 - (g_SmallFont->GetStringMetrics("Object Editor") + 4)) / 2, 80, "Object Editor", g_SmallFont.get());
   m_TitleGui->AddTextButton(GUI_TITLE_BUTTON_WORLD_EDITOR, (320 - (g_SmallFont->GetStringMetrics("World Editor") + 4)) / 2, 130, "World Editor", g_SmallFont.get());
   m_TitleGui->AddTextButton(GUI_TITLE_BUTTON_QUIT, (320 - (g_SmallFont->GetStringMetrics("Quit") + 4)) / 2, 190, "Quit", g_SmallFont.get());
   m_TitleGui->m_Active = true;
}

////////////////////////////////////////////////////////////////////////////////
///  UPDATERS
////////////////////////////////////////////////////////////////////////////////

void TitleState::UpdateTitle()
{
   m_TitleGui->Update();
   
   if(!m_TitleGui->m_Active)
      return;
   
   if(m_TitleGui->m_ActiveElement == GUI_TITLE_BUTTON_QUIT)
   {
      g_Engine->m_Done = true;
   }
   
   if(m_TitleGui->m_ActiveElement == GUI_TITLE_BUTTON_SINGLE_PLAYER)
   {
//      g_Server = new Server();
//      g_Server->Init("");
//      g_Client = new Client();
//      g_Client->Init("");
//      if(g_Client->Connect("127.0.0.1", g_Engine->m_EngineConfig["port"].numdata))
//      {
         //g_Server->m_NumberOfConnectedClients += 1;
      //}
      g_StateMachine->MakeStateTransition(STATE_MAINSTATE);
   }
   
   if(m_TitleGui->m_ActiveElement == GUI_TITLE_BUTTON_OBJECT_EDITOR)
   {
//      g_Server = new Server();
//      g_Server->Init("");
//      g_Client = new Client();
//      g_Client->Init("");
//      if(g_Client->Connect("127.0.0.1", g_Engine->m_EngineConfig["port"].numdata))
//      {
//         g_Server->m_NumberOfConnectedClients += 1;
//      }
      g_StateMachine->MakeStateTransition(STATE_OBJECTEDITORSTATE);
   }

   if(m_TitleGui->m_ActiveElement == GUI_TITLE_BUTTON_WORLD_EDITOR)
   {
//      g_Client = new Client();
//      g_Client->Init("");
      g_StateMachine->MakeStateTransition(STATE_WORLDEDITORSTATE);
      
   }
}
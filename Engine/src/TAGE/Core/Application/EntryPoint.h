#pragma once

#include "TAGE/Core/Application/Application.h"
extern TAGE::Application* TAGE::CreateApplication();

using namespace TAGE;
int main(int argc, char** argv)
{
	auto app = CreateApplication();
	app->Run();
	delete app;
}
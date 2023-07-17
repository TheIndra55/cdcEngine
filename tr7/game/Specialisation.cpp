#include "Specialisation.h"
#include "Main.h"

#include "game/local/localstr.h"
#include "game/resolve/Resolve.h"

Specialisation::Phase Specialisation::s_phase;
unsigned int Specialisation::s_targetMask;

void Specialisation::ContinueChange()
{
	if (s_phase != kDone)
	{
		auto fileSystem = GetFS();
		fileSystem->Update();

		s_phase = (Phase)(s_phase + 1);

		if (fileSystem->GetStatus() == cdc::FileSystem::IDLE && s_phase == kPostSync)
		{
			auto oldMask = fileSystem->GetSpecialisationMask();
			fileSystem->SetSpecialisationMask(s_targetMask);

			localstr_reload();

			Resolve::ChangeSpecialisation(oldMask, s_targetMask);
		}
	}
}

void Specialisation::BlockingChange(unsigned int newSpecMask)
{
	auto fileSystem = GetFS();

	if (fileSystem->GetSpecialisationMask() == newSpecMask)
	{
		if (s_phase == kDone)
		{
			return;
		}
	}
	else
	{
		s_phase = kPreSync;
		s_targetMask = newSpecMask;
	}

	while (s_phase != kDone)
	{
		ContinueChange();
	}
}
#pragma once

class Specialisation
{
private:
	enum Phase
	{
		kPreSync,
		kPostSync,
		kDone
	};

	static Phase s_phase;
	static unsigned int s_targetMask;

public:
	static void ContinueChange();
	static void BlockingChange(unsigned int newSpecMask);
};
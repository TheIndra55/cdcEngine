#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "Trace.h"

cdc::TraceLog cdc::g_defaultLog("", nullptr, cdc::TraceLog::Enabled);

unsigned int cdc::TraceLog::s_logCount;
cdc::TraceLog* cdc::TraceLog::s_logs[cdc::TraceLog::kMaxCount];
char cdc::TraceLog::s_effectiveFilters[cdc::TraceLog::kMaxCount];

cdc::TraceBackend* cdc::TraceBackend::s_backend = nullptr;

cdc::TraceLog::TraceLog(const char* name, TraceLog* parentLog, Filter filter)
{
	assert(s_logCount < kMaxCount);

	for (int i = 0; i < kMaxCount; i++)
	{
		auto log = s_logs[i];

		if (log)
		{
			if (strcmp(log->m_name, name) == 0)
			{
				assert(!"Log name is not unique");
			}
		}
	}

	auto str = new char[strlen(name) + 1];
	strcpy(str, name);

	m_name = str;
	m_parentLog = parentLog;
	m_index = s_logCount++;
	m_ownFilter = filter;

	s_logCount++;

	while (s_logs[m_index])
	{
		if (++m_index == kMaxCount)
		{
			m_index = 0;
		}
	}

	s_logs[m_index] = this;

	SetEffectiveFilter(filter == Default ? Disabled : filter);
}

cdc::TraceLog::~TraceLog()
{
	if (--s_logCount == 0)
	{
		auto backend = TraceBackend::Get();

		if (backend)
		{
			delete backend;
		}

		TraceBackend::Set(nullptr);
	}

	s_logs[m_index] = nullptr;
	s_effectiveFilters[m_index] = 0;

	delete m_name;
}

cdc::TraceLog* cdc::TraceLog::Get(unsigned int index)
{
	return s_logs[index];
}

unsigned int cdc::TraceLog::GetCount()
{
	return s_logCount;
}

bool cdc::TraceLog::CheckFilter(Mask mask)
{
	auto bits = mask.GetBits();

	while (bits != 0)
	{
		auto filter = s_effectiveFilters[bits & (kMaxCount - 1)];

		if (filter == Disabled)
		{
			return false;
		}

		bits >>= 10;
	}

	return true;
}

cdc::TraceLog::Filter cdc::TraceLog::GetOwnFilter()
{
	return m_ownFilter;
}

cdc::TraceLog::Filter cdc::TraceLog::GetEffectiveFilter()
{
	return (Filter)s_effectiveFilters[m_index];
}

void cdc::TraceLog::SetFilter(cdc::TraceLog::Filter filter)
{
	m_ownFilter = filter;

	SetEffectiveFilter(m_parentLog ? (Filter)s_effectiveFilters[m_parentLog->m_index] : Disabled);
}

cdc::TraceLog* cdc::TraceLog::GetParentLog()
{
	return m_parentLog;
}

const char* cdc::TraceLog::GetName()
{
	return m_name;
}

unsigned int cdc::TraceLog::GetIndex()
{
	return m_index;
}

void cdc::TraceLog::SetEffectiveFilter(Filter inheritedEffectiveFilter)
{
	auto filter = m_ownFilter == Disabled ? inheritedEffectiveFilter : m_ownFilter;

	s_effectiveFilters[m_index] = filter;

	for (int i = 0; i < kMaxCount; i++)
	{
		if (i == m_index) continue;

		auto log = s_logs[i];

		if (log && log->m_parentLog == this)
		{
			log->SetEffectiveFilter(filter);
		}
	}
}

cdc::TraceLog::Mask::Mask(unsigned int bits)
{
	m_bits = bits;
}

unsigned int cdc::TraceLog::Mask::GetBits()
{
	return m_bits;
}

cdc::TraceBackend* cdc::TraceBackend::Get()
{
	if (!s_backend)
	{
		s_backend = new TraceBackend_Std();
	}

	return s_backend;
}

void cdc::TraceBackend::Set(TraceBackend* backend)
{
	if (s_backend)
	{
		delete s_backend;
	}

	s_backend = backend;
}

cdc::TraceContext::TraceContext(TraceLog::Mask mask, bool autoTrailingNewline)
{
	m_filterCheckResult = TraceLog::CheckFilter(mask);
	m_autoTrailingNewline = autoTrailingNewline;
	m_staticDtorLog = nullptr;

	if (TraceLog::GetCount() == 0)
	{
		m_staticDtorLog = new TraceLog("StaticDtor", nullptr, TraceLog::Enabled);
	}

	if (m_filterCheckResult)
	{
		char buf[2048];

		auto opened = false;
		auto len = 0;

		for (int i = 0; i < 3; i++)
		{
			auto bits = mask.GetBits();
			auto index = (bits >> (10 * (2 - i))) & (TraceLog::kMaxCount - 1 );

			if (index)
			{
				auto log = TraceLog::Get(index);
				auto name = log->GetName();

				if (name[0])
				{
					len = cdc::FormattedAppend(buf, len, sizeof(buf), opened ? "|" : "[");
					len = cdc::FormattedAppend(buf, len, sizeof(buf), name);

					opened = true;
				}
			}
		}

		if (opened)
		{
			len = cdc::FormattedAppend(buf, len, sizeof(buf), "] ");
		}

		if (len == sizeof(buf)) len = sizeof(buf) - 1;
		buf[len] = 0;

		TraceBackend::Get()->Output(buf);
	}
}

bool cdc::TraceContext::GetFilterCheckResult()
{
	return m_filterCheckResult;
}

bool cdc::TraceContext::GetAutoTrailingNewline()
{
	return m_autoTrailingNewline;
}

void cdc::TraceContext::Destroy()
{
	if (m_filterCheckResult)
	{
		TraceBackend::Get()->Output("\n");
	}

	if (m_staticDtorLog)
	{
		delete m_staticDtorLog;
	}
}

void cdc::TraceBackend_Std::Output(const char* message)
{
	OutputDebugString(message);
}

void cdc::TraceCheckFilterAndPrint(cdc::TraceLog::Mask mask, const char* format, ...)
{
	va_list args;
	va_start(args, format);

	TraceContext context(mask, true);
	TracePartialV(&context, format, args);
	context.Destroy();
}

void cdc::TracePartialV(TraceContext* context, const char* format, va_list args)
{
	if (context->GetFilterCheckResult())
	{
		char buf[2048];
		buf[0] = 0;

		auto len = FormattedAppendV(buf, 0, sizeof(buf), format, args);

		if (context->GetAutoTrailingNewline() && len > 0 && buf[len - 1] == '\n')
		{
			buf[len - 1] = 0;
			len--;
		}

		if (len == sizeof(buf))
		{
			len = sizeof(buf) - 1;
			memcpy(&buf[sizeof(buf) - 4], "...", 3);
		}

		buf[len] = 0;

		TraceBackend::Get()->Output(buf);
	}
}

int cdc::FormattedAppend(char* buffer, int index, unsigned int size, char const* format, ...)
{
	va_list args;
	va_start(args, format);

	return FormattedAppendV(buffer, index, size, format, args);
}

int cdc::FormattedAppendV(char* buffer, int index, unsigned int size, char const* format, va_list args)
{
	auto len = vsnprintf(buffer + index, size - index, format, args);

	return len + index;
}
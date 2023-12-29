#pragma once

namespace cdc
{
	class TraceLog
	{
	public:
		enum Filter
		{
			Default,
			Enabled,
			Disabled
		};

		class Mask
		{
		public:
			Mask(unsigned int bits);

			unsigned int GetBits();

		private:
			unsigned int m_bits;
		};

		TraceLog(const char* name, TraceLog* parentLog, Filter filter);
		~TraceLog();

		static constexpr unsigned int kMaxCount = 1024;

		static TraceLog* Get(unsigned int index);
		static unsigned int GetCount();
		static bool CheckFilter(Mask mask);

		Filter GetOwnFilter();
		Filter GetEffectiveFilter();

		void SetFilter(cdc::TraceLog::Filter filter);

		TraceLog* GetParentLog();
		const char* GetName();
		unsigned int GetIndex();

	private:
		void SetEffectiveFilter(Filter inheritedEffectiveFilter);

		unsigned int m_index;
		cdc::TraceLog* m_parentLog;
		const char* m_name;
		Filter m_ownFilter;

		static unsigned int s_logCount;

		static TraceLog* s_logs[kMaxCount];
		static char s_effectiveFilters[kMaxCount];
	};

	class TraceBackend
	{
	public:
		virtual void Output(const char* message) = 0;

		static TraceBackend* Get();
		static void Set(TraceBackend* backend);

	private:
		static TraceBackend* s_backend;
	};

	class TraceContext
	{
	public:
		TraceContext(TraceLog::Mask mask, bool autoTrailingNewline);

		bool GetFilterCheckResult();
		bool GetAutoTrailingNewline();

		void Destroy();
		
	private:
		bool m_filterCheckResult;
		bool m_autoTrailingNewline;
		cdc::TraceLog* m_staticDtorLog;
	};

	class TraceBackend_Std : public TraceBackend
	{
	public:
		void Output(const char* message);
	};

	void TraceCheckFilterAndPrint(cdc::TraceLog::Mask mask, const char* format, ...);
	void TracePartialV(TraceContext* context, const char* format, va_list args);

	int FormattedAppend(char* buffer, int index, unsigned int size, char const* format, ...);
	int FormattedAppendV(char* buffer, int index, unsigned int size, char const* format, va_list args);

	extern TraceLog g_defaultLog;
}
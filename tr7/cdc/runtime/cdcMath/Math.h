#pragma once

namespace cdc
{
	template <typename T>
	T Clamp(T value, T minValue, T maxValue)
	{
		if (value > maxValue) value = maxValue;
		if (value < minValue) value = minValue;

		return value;
	}
}
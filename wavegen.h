#pragma once
#ifndef WAVEGEN_H
#define WAVEGEN_H

namespace wavegen
{
	constexpr double PI = 3.14159265358979323846;

	enum class WaveFunction
	{
		SINE,
		SAWTOOTH,
		SQUARE,
		TRIANGLE
	};

	double Sine(const double& freq, const double& x, const double& amplitude)
	{
		return sin(2.0 * PI * freq * x) * amplitude;
	}

	double Sawtooth(const double& freq, const double& x, const double& amplitude, const int& harmonics)
	{
		double y = 0.0;
		for (int k = 1; k <= harmonics; k++)
			y += sin(2.0 * PI * freq * k * x) / (double)k;
		return (2.0 * amplitude) / PI * y;
	}

	double Square(const double& freq, const double& x, const double& amplitude, const int& harmonics)
	{
		double y = 0.0;
		for (int k = 1; k <= harmonics; k++)
			y += sin(2.0 * PI * (2.0 * k - 1.0) * freq * x) / (2.0 * k - 1.0);
		return (4.0 / PI) * y * amplitude;
	}

	double Triangle(const double& freq, const double& x, const double& amplitude, const int& harmonics)
	{
		double y = 0.0;
		double n;
		for (int k = 0; k < harmonics; k++)
		{
			n = 2.0 * k + 1.0;
			y += pow(-1.0, k) * pow(n, -2.0) * sin(2.0 * PI * freq * n * x);
		}
		return (8.0 / pow(PI, 2.0)) * y * amplitude;
	}

	double Generate(WaveFunction f, const double& freq, const double& x, const double& amplitude, const int& harmonics = 1)
	{
		switch (f)
		{
		case WaveFunction::SINE: return Sine(freq, x, amplitude); break;
		case WaveFunction::SAWTOOTH: return Sawtooth(freq, x, amplitude, harmonics); break;
		case WaveFunction::SQUARE: return Square(freq, x, amplitude, harmonics); break;
		case WaveFunction::TRIANGLE: return Triangle(freq, x, amplitude, harmonics); break;
		default: return 0.0;
		}
	}
}

#endif /* ifndef WAVEGEN_H */
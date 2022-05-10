workspace "Terra Nova"
	architecture "x86"
	configurations { "Debug", "Release" }
	startproject "Terra Nova"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	include "Terra Nova"

#pragma once

#include <string>

/*
clasa care reprezinta o textura :-?
o clasa reprezinta o colectie de date specifice unui obiect
exemplu:
class persoana
{
public:
	persoana() <- functie chemata cand persoana se "naste"
	~persoana() <- functie chemata cand persoana moare

	// functii specifice unei persoane
	void mananca(tip_mancare mancare);
	void mergi(destinatie) 

private:
	char[] cnp
	int varsta
}

intr-o clasa putem incapsula (? parca era cuvantul?) variabilele si functiie, variabilele publice pot fi accesate de oriunde in afara clasei, cele private pot fi accesate doar
in interiorul clasei sau la clasele "prietene"

class persona
{
public:
	int varsta;
	void schimba_cnpul(char[] cnp2) {cnp = cnp2; /* <- pot face asta ca sunt in interiorul clasei}

private:
	char[] cnp;
};
persoana mara;

mara.varsta; <- e ok nu da eroare
mara.cnp; <- ups da eroare ca cnp-ul e un camp private si il pot accesa doar din clasa;

*/

class texture
{
public:
	// constructor- atunci cand fac un nou obiect de tipu "texture" se cheama constructorul declarat aici dar definit in "Texture.cpp"
	texture(const std::string& filepath);
	// deconstructor - cand se distruge un obiect de tipu asta se cheama 
	~texture();

	// da bind la textura intr-un slot primit ca parametru, am pus = 0 pt ca atunci cand chem functia pot sa chem doar texture.bind() si se da automat pe 0 sau pot sa chem
	// bind(1) si se da in slotu 1
	void bind(int slot = 0) const;

private:
	// variabile specifice unei texturi
	unsigned int texture_id;
	std::string texture_filepath;

	int width;
	int height;
	int bits_per_pixel;

	unsigned int format = 0;
	unsigned int internal_format = 0;
};
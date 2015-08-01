/*
Nom : 			Hellinckx
Prénom : 		Jérôme
Matricule :		000376500
Année d'étude : BA2 - 2014-2015
*/

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <exception>
#include <stdexcept>
#include "DimensionalArray.hpp"
#include "BoundedDimensionalArray.hpp"
#include "ArraySlice.hpp"


//---------------------------------------------------------------------------

void printException(const std::exception& e){ std::cerr << "\033[31m" << "*** " << e.what() << " ***" << "\033[0m" << std::endl; }

int main(){
	try{
		/* L'initialisation des tailles des dimensions se fait par orde décroissant */
		DimensionalArray<int, 4> a1({2,3,3,2}, 2);	//Initialise un vecteur d'entiers a 4 dimensions avec ses éléments mis a 2
		/* Problème de base */
		std::cout << a1 << std::endl;
		std::cout << "Taille de la dimension 2 : " << a1[0][0].length() << " == " << a1.length(2) << std::endl;
		std::cout << a1.size(2) << " == " << a1[0][0].size() << " éléments accessibles dans la dimension 2" << std::endl;
		std::cout << "Taille totale : " << a1.size() << std::endl;
		a1[1][1][1][1] = 10;
		std::cout << "Sous-vecteur modifié : " << a1[1][1][1] << std::endl;		//Affiche uniquement le sous-vecteur correspondant
		std::cout << a1 << std::endl;
		//Pour le redimensionnement on oblige de fournir la taille de toutes les dimensions
		try { std::cout << "Redimensionnement incomplet : "; a1.resize({1,1}); } catch (const std::exception& e){ printException(e); }
		std::cout << "Redimensionnement complet : ";
		a1.resize({3,2,3,3}); //Les précédentes valeurs sont conservées au possible, sinon constructeur par défaut de Elem appelé
		std::cout << a1 << std::endl;
		DimensionalArray<int, 4> a2({1,1,1,1}, 5);
		a1 = a2;
		std::cout << "Redimensionnement implicite : "<< a1 << std::endl;
		a1[0][0][0][0] = 100;
		std::cout << "Modification d'un élément après assignation : " << a1[0][0][0][0] << " <-> " << a2[0][0][0][0] << std::endl;
		DimensionalArray<char, 1> a3(10, 'c'); //Instanciation avec char
		std::cout << a3 << std::endl;
		try { std::cout << "Valeur négative pour l'opérateur [] : "; a3[-4] = 1; } catch (const std::exception& e){ printException(e); }
		
		/* Sous-question 1 */
		//Il manque des bornes pour au moins une dimension
		try { std::cout << "Constructeur incomplet d'un vecteur borné : "; BoundedDimensionalArray<int, 3> b1({{1},{1},{1}}); } catch (const std::exception& e){ printException(e); }
		//Les bornes ne sont pas données pour chaque dimension
		try { std::cout << "Constructeur incomplet d'un vecteur borné : "; BoundedDimensionalArray<int, 3> b1({{1,2},{1,2}}); } catch (const std::exception& e){ printException(e); }
		BoundedDimensionalArray<int, 3> b1({{1,3},{-3,-2},{3,5}}, 3);
		std::cout << "Contructeur complet d'un vecteur borné : " << b1 << std::endl;
		std::cout << "Bornes pour dimension 2 : " << "(" << 	b1.lower(2) << "==" << b1[1].lower() << "," << 
																b1.upper(2) << "==" << b1[1].upper() << ")" << std::endl;
		b1[1][-3][5] = 100;
		b1[1][-2] = b1[1][-3];
		std::cout << "Accès dans les bornes et copie de sous-vecteur : " << b1[1][-2] << std::endl;
		try { std::cout << "Accès hors des bornes : "; b1[3][-4][4] = 2; } catch (const std::exception& e){ printException(e); }
		DimensionalArray<int, 3> b2(b1);
		std::cout << "Vecteur non-borné construit à partir d'un vecteur borné : " << b2 << std::endl;
		//BoundedDimensionalArray<int, 2> b3(b2); ERREUR, DimensionalArray n'est pas borné

		/* Sous-question 2 */
		//Avec tableau non-borné
		DimensionalArray<double, 3> c1({3,3,4}, 5.3);
		/* Des exceptions sont levées si la construction du slice est incorrecte, 
		semblables à celles levées lors de la construction d'un vecteur borné */
		auto slice1 = c1.slice({{0,2},{0,2},{0,2}}); //auto == ArraySlice<double, 3> et pas de 1 par défaut
		std::cout << "Slice : " << slice1 << std::endl;
		std::cout << "Avant modification dans le slice : " << c1[1][1] << std::endl;
		slice1[1][1][1] = 0;
		std::cout << "Après modification dans le slice : " << c1[1][1] << std::endl;
		try { std::cout << "Hors bornes : "; slice1[1][1][3] = 5; } catch (const std::exception& e){ printException(e); }
		auto slice2 = c1.slice({{0,2,2},{0,2,2},{0,2,2}}); //même slice avec pas de 2 (3ème element dans chaque liste d'init)
		std::cout << "Slice de pas 2 : " << slice2 << std::endl;
		std::cout << "Avant modification dans le slice de pas 2 : " << slice1[2][2] << std::endl;
		slice2[2][2][2] = 30;
		std::cout << "Après modification dans le slice de pas 2 : " << slice1[2][2] << std::endl;
		try { std::cout << "Hors bornes avec pas de 2 : "; slice2[1][1][1] = 5; } catch (const std::exception& e){ printException(e); }
		//Avec tableau borné
		BoundedDimensionalArray<int, 2> c2({{1,3},{4,6}},1);
		auto slice3 = c2.slice({{2,3},{5,6}}); //Les bornes données doivent être dans les bornes de c2 sinon exception
		std::cout << "Slice borné : " << slice3 << std::endl;
		std::cout << "Avant modification dans le slice borné : " << c2[2] << std::endl;
		slice3[2][5] = 8;
		std::cout << "Après modification dans le slice borné : " << c2[2] << std::endl;
		//Slice vers nouveau tableau borné
		BoundedDimensionalArray<int, 2> c3(slice3);
		std::cout << "Slice copié : " << slice3 << std::endl;
		std::cout << "Vecteur borné obtenu : " << c3 << std::endl;
		std::cout << "Avant modification dans le slice copié : " << c3[2] << std::endl;
		slice3[2][6] = 8;
		std::cout << "Après modification dans le slice copié : " << c3[2] << std::endl;
		std::cout << "Et slice copié : " << slice3[2] << std::endl;
	} catch(const std::exception& e){
		printException(e);
		return 1;
	}
	return 0;
}

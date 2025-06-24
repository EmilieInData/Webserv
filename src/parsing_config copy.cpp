/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_config copy.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esellier <esellier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 19:21:07 by esellier          #+#    #+#             */
/*   Updated: 2025/06/24 15:54:38 by esellier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


//->checker qu il y a bien un listen si le array est vide en mettre un par default


	//comparer avec nginx pour les messages d'erreurs
	
	//checker si une ligne est si grande que la fonction me la retourne en deux fois ? 

	//server tjr suivi d'une parenthese ou saut de ligne
	//location tjr suivi d'un token qui n'est pas une parenthese


// std::string&	checkSemicolon(std::string& line)
// {
// 	size_t			j;
	
// 	j = line.find(';');
// 	if (j == std::string::npos && !first.empty()) // si il n'y a pas
// 	{ // si server ou location, pas avant parenthese et pas juste apres
// 		if (first == "server" || first == "location" || first == "{" || first == "}")
// 			return line; // manque de checker le mot precedent
// 		throw std::invalid_argument("Parsing error, miss semicolon at the end of the line\n");

// 	}
// 	for (size_t i = j + 1; i < line.length(); i++)  //si il y a autre chose derriere
// 	{
// 		if (line[i] != 9 && line[i] != 32 && line[i] != '}')
// 			throw std::invalid_argument("Parsing error, no argument accepted after the semicolon\n");
// 		if (line[i] == '}') //parenthese de fin de ligne acceptee uniquement 
// 		{
// 			i++;
// 			while (line[i])
// 			{
// 				if (line[i] != 9 && line[i] != 32)
// 					throw std::invalid_argument("Parsing error, no argument accepted after the brace\n");
// 				i++;
// 			}
// 			break;
// 		}
// 	}
	
// 	size_t i = 0;
// 	while (line[i] && line[i] < 33)
// 		i++;
// 	if (line[i] == ';') //pas de semicolon en premier token
// 		throw std::invalid_argument("Parsing error, need argument before the semicolon\n");
// 	// si server ou location, pas avant parenthese et pas juste apres
// 	// if (first == "server" || first == "location" || first == "{" || first == "}")
// 	// {
// 	// 	size_t tmp = line.find('{');
// 	// 	size_t tmp2 = line.find('}');

// 	// 	//if ((tmp != std::string::npos && tmp < j ++ un arg entre les deux ) || (tmp2 != std::string::npos && j < tmp2));
		
// 	// 	//throw std::invalid_argument("Parsing error, semicolon not alloweed in bloc declaration\n");
		
// 	// }
// 	line.insert(j, " ");//mettre un espace avant le ";" pour le garder ds un autre token
// 	return line;
// }

// Si la ligne commence par un mot-clé de bloc (server, location, etc.)
// Et contient un point-virgule :
// avant la première { → ❌ erreur
// juste après la { (comme {;) → ❌ erreur


//[config error] 'cgi_pass'or 'allow_mathods' directive must be inside a location block (line X)
//ligne dupliquee
//bloc pas fermee
//faute de frappe, mauvaises directives
//[config error] Invalid size format in 'client_max_body_size': 12Z (line X)
//manque des infos, comme listen..
//mauvais type de valeur pour chaque directive
//deux blocs location avec la mm direction (checker la clef avant d'en creer une nouvelle)
//point virgule

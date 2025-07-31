/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Graphics.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:09:44 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/07/31 16:53:53 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "Utils.hpp"

#define LINELENGTH 60

#define PROJ_TITLE "le webserv fantastique - 42 bcn"

#define TEAM_LIST "cle-tron - esellier - fdi-cecc"

#define LEFT_SIDE "| "
#define PAD "+"
#define RIGHT_SIDE " | |"
#define SYMBOL "  > "

void	graSimpleLine(char const &c);
void	graSeparator(char const &c);
void	graTextLine(std::string const &str);
void	graTextHeader(std::string const &str);
void	graTextElement(std::string const &str);

#endif

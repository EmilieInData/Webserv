/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Graphics.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:09:44 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/08/03 12:11:37 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "Utils.hpp"

#define LINELENGTH 60
#define PADDING 1
#define PROJ_TITLE "le webserv fantastique - 42 bcn"

#define TEAM_LIST "cle-tron - esellier - fdi-cecc"

#define TOP_LEFT "╔"
#define TOP_RIGHT "╗"
#define MID_LEFT "╠"
#define MID_RIGHT "╣"
#define BOT_LEFT "╚"
#define BOT_RIGHT "╝"
#define VERTICAL "║"
#define HORIZONTAL "═"
#define SYMBOL "   > "

void		graBottomLine();
void		graTopLine();
void		graSeparator();
void		graEmptyLine();
void		graTextLine(std::string const &str);
void		graTextHeader(std::string const &str);
void		graTextElement(std::string const &str);
void		graTime(std::string const &str);
void		graTime();
std::string	graPad(std::string const &str);

#endif

/* ═ ║ ╔ ╗ ╚ ╝ ╠ ╣ ╦ ╩ ╬ */
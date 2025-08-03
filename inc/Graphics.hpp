/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Graphics.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdi-cecc <fdi-cecc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:09:44 by fdi-cecc          #+#    #+#             */
/*   Updated: 2025/08/03 18:30:53 by fdi-cecc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "Utils.hpp"

#define LINELENGTH 60
#define PADDING 1

#define WEBSERV_BANNER \
"\n" \
"              ,,,,,,\n" \
"          o#'9MMHb':'-,o,\n" \
"       .oH\":HH$' \"' ' -*R&o,\n" \
"      dMMM*\"\"'`'      .oM\"HM?.\n" \
"    ,MMM'          \"HLbd< ?&H\\\n" \
"   .:MH .\"          ` MM  MM&b\n" \
"  . \"*H    -        &MMMMMMMMMH:\n" \
"  .    dboo        MMMMMMMMMMMM.  le_webserv_fantastique\n" \
"  .   dMMMMMMb      *MMMMMMMMMP.  42_bcn\n" \
"  .    MMMMMMMP        *MMMMMP .  cle-tron_esellier_fdi-cecc\n" \
"       `#MMMMM           MM6P ,\n" \
"   '    `MMMP\"           HM*`,\n" \
"    '    :MM             .- ,\n" \
"     '.   `#?..  .       ..'\n" \
"        -.   .         .-\n" \
"          ''-.oo,oo.-''\n"

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
void		graTextElement(int const &num);
void		graTime(std::string const &str);
void		graTime();
std::string	graPad(std::string const &str);

#endif

/* ═ ║ ╔ ╗ ╚ ╝ ╠ ╣ ╦ ╩ ╬ */
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuhrman <mfuhrman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 12:23:08 by mfuhrman          #+#    #+#             */
/*   Updated: 2023/05/16 14:03:54 by mfuhrman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include <fstream>
# include <iostream>
# include <cstring>
# include <vector>
# include <algorithm>


# ifndef COLOR
#  define COLOR
#  define CLEAR		"\x1B[2J\x1B[H"
#  define NONE		"\033[0;37m"
#  define WHITE		"\033[1;37m"
#  define RED		"\033[1;31m"
#  define GREEN		"\033[1;32m"
#  define CYANE		"\033[1;36m"
#  define ORANGE	"\033[1,38m"
#  define YELLOW	"\033[1;33m"
#  define BLUE		"\033[1,34m" 
# endif

typedef struct s_args
{
    std::string  fileName;
    int level;
    std::string path;
}   t_args;

typedef struct s_pageHTML
{
    std::string 				pageHTML;
    std::string                 url;
	std::vector<std::string>	tabImg;
	std::vector<std::string>	tabHref;
    int	                        nbImg;
    int	                        nbHref;
    std::string                 fileTxt;
    std::string	                path;
}   t_pageHTML;


void	    ft_getPage(t_pageHTML &datas);       // Stocke un fichier .txt dans une std::string
void        ft_parsingImg(t_pageHTML &datas);
void        ft_parsingHref(t_pageHTML &datas);
std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);
void	    findAll(t_pageHTML	&datas);
void parseArgs(t_args &args, char **av, int ac);

#endif
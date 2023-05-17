/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuhrman <mfuhrman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 12:25:59 by mfuhrman          #+#    #+#             */
/*   Updated: 2023/05/17 08:13:28 by mfuhrman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

int main(int ac, char **av){

	
//Si un seul argument on prend l'url

if (ac == 2)
{
	t_args arg;
	t_pageHTML	datas;
	datas.url = av[1];
	datas.fileTxt = "page.txt";
	datas.path = "data";
	arg.level = 5;
	int count = 0;
	
	std::string	pathFinal = "mkdir " + datas.path;
	system(pathFinal.c_str());
	findAll(datas);
	std::cout << "\n *********   test  **************" << std::endl;


	while (count <= arg.level)
	{
	 	for (std::vector<std::string>::iterator it = datas.tabHref.begin(); it != datas.tabHref.end(); it++)
	 	{
	 		if (it->at(it->length() - 1) != '/')
				datas.url = *it + "/";
			else
	 			datas.url = *it;


	 		std::cout << "\n *********" << datas.url << " **************\n" << std::endl;

	
	 		findAll(datas);
		}
		count++;
	}
	return 0;
}

//Avec 2 arguments

else if (ac == 3)
{
	t_args args; 
	parseArgs(args, av, ac);
	std::cout << BLUE << "fileName: " << NONE << args.fileName << std::endl;
	std::cout << GREEN << "url: " << NONE << args.path << std::endl;
	t_pageHTML	datas;
	datas.url = args.path;
	datas.fileTxt = "page.txt";
	datas.path = args.fileName;
	std::string	pathFinal = "mkdir " + datas.path;
	system(pathFinal.c_str());




	findAll(datas);
	std::cout << "\n *********   test  **************" << std::endl;
}
else
	std::cout << RED << "Error: wrong arguments !" << NONE << std::endl;
	
}

void	findAll(t_pageHTML	&datas)
{
	std::string	requestUrl = "curl " + datas.url + " >> " + datas.fileTxt;

	// *******  Request URL *****
	
	system(requestUrl.c_str());
	ft_getPage(datas);
	system("rm page.txt");

	// ******** stocke url des images dans std::vector<std::string>
	ft_parsingImg(datas);

	// ******** stocke url des href dans std::vector<std::string>
	ft_parsingHref(datas);

	// ******** test list url des images
	std::cout << "\n *********   images  **************" << std::endl;

	for (std::vector<std::string>::iterator it = datas.tabImg.begin(); it != datas.tabImg.end(); it++)
	{
		std::cout << *it << std::endl;
	}
	// ******** test list url des href
	std::cout << "\n *********   href  **************" << std::endl;
	for (std::vector<std::string>::iterator it = datas.tabHref.begin(); it != datas.tabHref.end(); it++)
	{
		std::cout << *it << std::endl;
	}

	// Dowload les images dans /stock_images
	int i = 0;
	std::string	nameImg;
	for (std::vector<std::string>::iterator it = datas.tabImg.begin(); it != datas.tabImg.end(); it++)
	{
		nameImg = *it;
		nameImg = nameImg.substr(nameImg.rfind("/"));
		//requestUrl = "curl -o ./stock_images/" + nameImg + " " + *it;
		requestUrl = "curl -o ./" + datas.path + "/" + nameImg + " " + *it;
		system(requestUrl.c_str());
		i++;
	}


	// for (std::vector<std::string>::iterator it = datas.tabHref.begin(); it != datas.tabHref.end(); it++)
	// {
	// 	datas.url = *it;
	// 	findAll(datas);
	// }
	



}

void	ft_parsingHref(t_pageHTML &datas){
	size_t pos = 0;
	size_t posEnd = 0;
	size_t index = 0;
	std::string	urlImg = "";

	datas.nbHref = 0;

	while (true)
	{
		index = datas.pageHTML.find("<a", pos);
		if (index == std::string::npos)
			break;
		pos = index;
		index = datas.pageHTML.find("href=", pos);
		if (index == std::string::npos)
		{
			std::cerr << " *****  error    ********\nhref no found !!!" << std::endl;
			break;
		}

		pos = index + 6;
		posEnd = datas.pageHTML.find("\"", pos);
		urlImg = datas.pageHTML.substr(pos, posEnd - pos);

		// ***********  stock les href dans std::vector<std::string>
		{
			size_t pos2 = urlImg.find("http", 0);
			if (pos2 == 0)
			{
				datas.tabHref.push_back(urlImg);
			}
			else if (urlImg.at(0) == '/')
				datas.tabHref.push_back(datas.url + urlImg.substr(1));
			else if (urlImg.at(0) == '.')
				datas.tabHref.push_back(datas.url + urlImg.substr(2));
			else
				datas.tabHref.push_back(datas.url + urlImg);
			urlImg = "";

			datas.nbHref += 1;
		}


	}
}


void	ft_parsingImg(t_pageHTML &datas){
	size_t pos = 0;
	size_t posEnd = 0;
	size_t index = 0;
	std::string	urlImg = "";

	datas.nbImg = 0;

	while (true)
	{
		index = datas.pageHTML.find("<img", pos);
		if (index == std::string::npos)
			break;
		pos = index;
		index = datas.pageHTML.find("src=", pos);
		if (index == std::string::npos)
		{
			std::cerr << " *****  error    ********\nsrc no found !!!" << std::endl;
			break;
		}

		pos = index + 5;
		posEnd = datas.pageHTML.find("\"", pos);
		urlImg = datas.pageHTML.substr(pos, posEnd - pos);

		// ***********  stock les images dans std::vector<std::string>

		if (!(urlImg.rfind(".jpg") == std::string::npos  && urlImg.rfind(".jpeg") == std::string::npos
				&& urlImg.rfind(".gif") == std::string::npos
				&& urlImg.rfind(".png") == std::string::npos
				&& urlImg.rfind(".bmp") == std::string::npos))
		{
			size_t pos2 = urlImg.find("http", 0);
			if (pos2 == 0)
			{
				if (urlImg.find("wordpress.") != std::string::npos)
				{
					urlImg = ReplaceAll(urlImg, "wordpress", "www");
					datas.tabImg.push_back(urlImg);
				}
				else
					datas.tabImg.push_back(urlImg);
			}
			else if (urlImg.at(0) == '/')
				datas.tabImg.push_back(datas.url + urlImg.substr(1));
			else if (urlImg.at(0) == '.')
				datas.tabImg.push_back(datas.url + urlImg.substr(2));
			else
				datas.tabImg.push_back(datas.url + urlImg);
			urlImg = "";

			datas.nbImg += 1;
		}


	}
}

void	ft_getPage(t_pageHTML	&datas)
{
	std::string	filename = datas.fileTxt;
	datas.pageHTML = "";
	char	c;

	std::ifstream	ifs(filename, std::ios::in);

	if (!ifs)
	{
		std::cout << "Error file in" << std::endl;
		exit(0);
	}

	while (ifs.get(c))
		datas.pageHTML += c;

	ifs.close();

	//return source;
}


std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}


//Parse les arguments (nom du dossier)


void parseArgs(t_args &args, char **av, int ac)
{
	int i = 0;
	while (i < ac)
	{
		if (strncmp("-p", av[i], 2) == 0)
		{
			args.fileName = av[i];
			args.fileName = args.fileName.substr(2, std::string::npos);

		}
			
		else if ( strlen(av[i]) > 0 && av[i][0] != '.')
			args.path = av[i];
		i++;
	}
}

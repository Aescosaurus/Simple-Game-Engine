#include <iostream>
#include "ModuleHandler.h"
#include <algorithm>
#include "EntityHandler.h"
#include "Builder.h"

std::vector<std::string> parse_input( const std::string& input );

int main()
{
	static constexpr auto tab = "    ";
	ModuleHandler modHand;
	EntityHandler entityHand{ modHand };
	Builder builder;

	std::string input = "";
	while( input != "quit" )
	{
		std::cout << "Type a command (type help for help): ";
		std::getline( std::cin,input );

		std::vector<std::string> command = parse_input( input );

		if( command[0] == "help" )
		{
			std::ifstream in{ "Help.txt" };
			while( in.good() )
			{
				std::string line;
				std::getline( in,line );
				std::cout << tab << "- " << line << '\n';
			}
		}
		else if( command[0] == "listentity" )
		{
			const auto& entityList = entityHand.ListEntities();
			std::cout << "Listing " << entityList.size() << " entities:\n";
			for( const auto& entity : entityList )
			{
				std::cout << tab << entity << '\n';
			}
		}
		else if( command[0] == "listmodule" )
		{
			const auto& modList = modHand.ListModules();
			std::cout << "Listing " << modList.size() << " modules:\n";
			for( const auto& mod : modList )
			{
				std::cout << tab << mod << '\n';
			}
		}
		else if( command[0] == "showcode" && command.size() == 2 )
		{
			if( modHand.ModuleExists( command[1] ) )
			{
				for( const auto& line : modHand.ReadModuleCode( command[1] ) )
				{
					std::cout << tab;
					for( char c : line )
					{
						if( c == '\t' ) std::cout << tab;
						else std::cout << c;
					}
					std::cout << '\n';
					// std::cout << line << '\n';
				}
				// std::cout << modHand.ReadModuleCode( command[1] ) << '\n';
			}
			else std::cout << "Module not found!\n";
		}
		else if( command[0] == "createentity" && command.size() == 2 )
		{
			if( !entityHand.EntityExists( command[1] ) )
			{
				entityHand.CreateEntity( command[1] );
				std::cout << "Entity created!\n";
			}
			else std::cout << "Entity with that name already exists!\n";
		}
		else if( command[0] == "addmodule" && command.size() == 3 )
		{
			if( entityHand.EntityExists( command[1] ) &&
				modHand.ModuleExists( command[2] ) )
			{
				entityHand.AddModule( command[1],command[2] );
				std::cout << "Module added successfully!\n";
			}
			else std::cout << "Entity or module does not exist!\n";
		}
		else if( command[0] == "showentity" && command.size() == 2 )
		{
			if( entityHand.EntityExists( command[1] ) )
			{
				std::cout << "Showing " << command[1] << " modules:\n";
				for( const auto& mod : entityHand.ShowEntity( command[1] ) )
				{
					std::cout << tab << mod << '\n';
				}
				// std::cout << entityHand.ShowEntity( command[1] ) << '\n';
			}
			else std::cout << "Entity does not exist!\n";
		}
		else if( command[0] == "showentityarg" && command.size() == 2 )
		{
			if( entityHand.EntityExists( command[1] ) )
			{
				std::cout << "Showing " << command[1] << " modules with arguments:\n";
				for( const auto& mod : entityHand.ShowEntityLong( command[1] ) )
				{
					for( int i = 0; i < int( mod.size() ); ++i )
					{
						if( i > 0 ) std::cout << tab;
						std::cout << tab << mod[i] << '\n';
					}
				}
			}
			else std::cout << "Entity does not exist!\n";
		}
		else if( command[0] == "build" )
		{
			builder.Build( modHand,entityHand );
			std::cout << "Build finished!\n";
		}
		else if( command[0] == "deleteentity" && command.size() == 2 )
		{
			if( entityHand.EntityExists( command[1] ) )
			{
				entityHand.DeleteEntity( command[1] );
				std::cout << "Entity deleted!\n";
			}
			else std::cout << "Entity does not exist.\n";
		}
		else if( command[0] == "setarg" && command.size() == 5 )
		{
			if( entityHand.EntityExists( command[1] ) )
			{
				const auto modList = entityHand.ShowEntity( command[1] );
				if( std::find( modList.begin(),modList.end(),command[2] ) != modList.end() )
				{
					const auto test = entityHand.RemoveModNumber( command[2] );
					const auto& argList = modHand.GetModuleArgs(
						entityHand.RemoveModNumber( command[2] ) );
					if( std::find( argList.begin(),argList.end(),command[3] ) != argList.end() )
					{
						entityHand.SetArg( command[1],command[2],command[3],command[4] );
						std::cout << "Argument set successfully!\n";
					}
					else std::cout << "Argument does not exist on this module!\n";
				}
				else std::cout << "Module does not exist on this entity!\n";
			}
			else std::cout << "Entity does not exist!\n";
		}
		// TODO: delete and rename mod/entity
		// else if
		else if( command[0] == "quit" )
		{

		}
		else
		{
			std::cout << "Invalid command! " << input << '\n';
		}
	}

	return( 0 );
}

std::vector<std::string> parse_input( const std::string& input )
{
	std::vector<std::string> wordList;
	wordList.emplace_back( "" );
	for( char c : input )
	{
		if( c == ' ' ) wordList.emplace_back( "" );
		else wordList.back() += c;
	}
	return( wordList );
}
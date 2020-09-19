#pragma once

#include <string>
#include <fstream>
#include <filesystem>
#include <cctype>
#include "ModuleHandler.h"

class EntityHandler
{
public:
	EntityHandler( ModuleHandler& modHand )
		:
		modHand( modHand )
	{
	}

	void CreateEntity( const std::string& name )
	{
		if( !EntityExists( name ) )
		{
			std::ofstream out{ entityPath + name + ex };
		}
	}

	void DeleteEntity( const std::string& entityName )
	{
		if( EntityExists( entityName ) )
		{
			std::filesystem::remove( entityPath + entityName + ex );
		}
	}

	void AddModule( const std::string& entityName,const std::string& modName )
	{
		int dupeCount = 0;
		for( const auto& mod : ShowEntity( entityName ) )
		{
			if( RemoveModNumber( mod ) == modName )
			{
				++dupeCount;
			}
		}

		std::ofstream out{ entityPath + entityName + ex,std::ios_base::app };
		out << modName << dupeCount;
		for( const auto& arg : modHand.GetModuleArgs( modName ) )
		{
			out << " 0";
		}
		out << '\n';
	}

	// Mod name needs to include number suffix.
	void SetArg( const std::string& entityName,const std::string& modName,
		const std::string& argName,const std::string& val )
	{
		std::vector<std::string> args;
		for( const auto& mod : ShowEntityLong( entityName ) )
		{
			if( mod[0] == modName )
			{
				for( const auto& arg : mod )
				{
					args.emplace_back( RemoveArgName( arg ) );
					if( args.back().length() < 1 ) args.pop_back();
				}
			}
		}

		const auto& argNames = modHand.GetModuleArgs( RemoveModNumber( modName ) );
		for( int i = 0; i < int( argNames.size() ); ++i )
		{
			if( argNames[i] == argName )
			{
				args[i] = val;
			}
		}

		std::string result = "";
		std::ifstream in{ entityPath + entityName + ex };
		while( in.good() )
		{
			std::string line;
			std::getline( in,line );
			if( line.substr( 0,modName.length() ) == modName )
			{
				result += modName;
				for( const auto& curArg : args )
				{
					result += " " + curArg;
				}
			}
			else result += line;

			result += '\n';
		}

		for( int i = int( result.size() ) - 1; i > 0; --i )
		{
			if( result[i] == '\n' ) result.pop_back();
			else break;
		}

		result += '\n';

		in.close();
		std::ofstream out{ entityPath + entityName + ex };
		out << result;
	}

	std::vector<std::string> ListEntities() const
	{
		std::vector<std::string> names;
		for( const auto& entry : std::filesystem::directory_iterator( entityPath ) )
		{
			const auto entityName = entry.path().filename().string();
			names.emplace_back( entityName.substr( 0,entityName.find_last_of( '.' ) ) );
		}
		return( names );
	}

	std::vector<std::string> ShowEntity( const std::string& entityName ) const
	{
		std::vector<std::string> result;
		if( EntityExists( entityName ) )
		{
			std::ifstream in{ entityPath + entityName + ex };
			while( in.good() )
			{
				std::string curLine;
				std::getline( in,curLine );
				if( curLine.length() > 0 )
				{
					result.emplace_back( "" );
					for( char c : curLine )
					{
						if( c == ' ' ) break;
						result.back() += c;
					}
				}
			}
		}
		else result.emplace_back( "Entity not found!" );

		return( result );
	}

	// Includes argument list.
	std::vector<std::vector<std::string>> ShowEntityLong( const std::string& entityName ) const
	{
		std::vector<std::vector<std::string>> result;
		if( EntityExists( entityName ) )
		{
			std::ifstream in{ entityPath + entityName + ex };
			while( in.good() )
			{
				std::string curLine;
				std::getline( in,curLine );
				result.emplace_back();
				if( curLine.length() > 0 )
				{
					result.back().emplace_back( "" );
					std::vector<std::string> argNames;
					int curArg = 0;
					for( char c : curLine )
					{
						if( c == ' ' )
						{
							if( argNames.size() < 1 )
							{
								for( const auto& arg : modHand.GetModuleArgs(
									RemoveModNumber( result.back().back() ) ) )
								{
									argNames.emplace_back( arg );
								}
							}
							// result.back().emplace_back( "\n" );
							result.back().emplace_back( argNames[curArg++] + ": " );
						}
						else
						{
							result.back().back() += c;
						}
					}
				}
			}
		}
		else result.emplace_back().emplace_back( "Entity not found!" );

		if( result.back().size() < 1 ) result.pop_back();

		return( result );
	}

	bool EntityExists( const std::string& entityName ) const
	{
		std::ifstream in{ entityPath + entityName + ex };
		return( in.good() );
	}

	std::string RemoveModNumber( const std::string& modWithNum ) const
	{
		int subSpot = modWithNum.length();

		for( int i = 0; i < int( modWithNum.length() ); ++i )
		{
			if( isdigit( modWithNum[i] ) )
			{
				subSpot = i;
				break;
			}
		}

		return( modWithNum.substr( 0,subSpot ) );
	}

	std::string RemoveArgName( const std::string& argWithName ) const
	{
		std::string replacement = "";
		bool canReplace = false;
		for( char c : argWithName )
		{
			if( c == ' ' ) canReplace = true;
			else if( canReplace )
			{
				replacement += c;
			}
		}
		
		return( replacement );
	}
private:
	static constexpr auto entityPath = "Entities/";
	static constexpr auto ex = ".txt";
	ModuleHandler& modHand;
};
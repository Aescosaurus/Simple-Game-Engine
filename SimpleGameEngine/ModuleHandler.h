#pragma once

#include <string>
#include <filesystem>
#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>

class ModuleHandler
{
public:
	ModuleHandler()
	{
		for( const auto& entry : std::filesystem::directory_iterator( modulePath ) )
		{
			const auto modName = entry.path().filename().string();
			moduleNames.emplace_back( modName.substr( 0,modName.find_last_of( '.' ) ) );
		}
	}

	const std::vector<std::string>& ListModules() const
	{
		return( moduleNames );
	}

	std::vector<std::string> GetModuleArgs( const std::string& modName ) const
	{
		std::vector<std::string> args;
		std::ifstream in{ modulePath + modName + ex };
		assert( in.good() );

		std::string line;
		std::getline( in,line ); // name
		std::getline( in,line ); // desc
		std::getline( in,line ); // args

		for( char c : line )
		{
			if( c == ' ' )
			{
				args.emplace_back( "" );
			}
			else if( args.size() > 0 )
			{
				args.back() += c;
			}
		}

		return( args );
	}

	std::vector<std::string> ReadModuleCode( const std::string& name ) const
	{
		std::vector<std::string> result;
		std::ifstream in{ modulePath + name + ex };

		if( ModuleExists( name ) )
		{
			while( in.good() )
			{
				// const auto c = in.get();
				// if( c == '\t' ) result += "    ";
				// else result += c;
				std::string line;
				std::getline( in,line );
				result.emplace_back( line );
			}
		}
		else result.emplace_back( "Module not found!" );
		
		return( result );
	}

	bool ModuleExists( const std::string& modName ) const
	{
		std::ifstream in{ modulePath + modName + ex };
		return( in.good() );
	}
public:
	static constexpr auto modulePath = "Modules/";
	static constexpr auto ex = ".js";
private:
	std::vector<std::string> moduleNames;
};
#pragma once

#include <fstream>
#include <string>
#include "ModuleHandler.h"
#include "EntityHandler.h"
#include <map>
#include <cctype>

class Builder
{
public:
	void Build( const ModuleHandler& modHand,
		const EntityHandler& entityHand )
	{
		// Delete old build.
		Cleanup();
		
		// index.html
		WriteIndex( entityHand,modHand );

		// Main.js, encapsulates all entities and runs them
		WriteMain( entityHand );

		// Writes all entities
		WriteEntities( entityHand,modHand );

		// Copies over existing modules.
		WriteModules( modHand );

		// Copies over existing framework files.
		CopyFramework();
	}
private:
	void WriteIndex( const EntityHandler& entityHand,
		const ModuleHandler& modHand )
	{
		const std::string indexPath = "index.html";
		std::ofstream out{ buildPath + indexPath };

		out << "<!DOCTYPE html>\n";
		out << "<html>\n";

		out << "\t<head>\n";

		for( const auto& entry : std::filesystem::directory_iterator( frameworkPath ) )
		{
			out << "\t\t<script src=\"" << entry.path().filename().string() << "\"></script>\n";
		}
		// out << "\t\t<script src=\"EntityBase.js\"></script>\n";
		// out << "\t\t<script src=\"ModuleBase.js\"></script>\n";
		// out << "\t\t<script src=\"Graphics.js\"></script>\n";

		out << '\n';

		const auto modules = modHand.ListModules();
		for( const auto& mod : modules )
		{
			out << "\t\t<script src=\"" + mod + ModuleHandler::ex + "\"></script>\n";
		}

		out << '\n';

		for( const auto& entity : entityHand.ListEntities() )
		{
			out << "\t\t<script src=\"" + entity + ".js\"></script>\n";
		}

		// out << '\n';

		// out << "\t\t<script src=\"Main.js\"></script>\n";

		out << "\t</head>\n";

		out << "\n\t<body>\n";

		out << "\t\t<canvas id=\"canv\" width=\"960\" height = \"540\"></canvas>\n";

		out << "\t\t<script src=\"Main.js\"></script>\n";

		out << "\t</body>\n";

		out << "</html>\n";
	}

	void WriteMain( const EntityHandler& entityHand )
	{
		std::ofstream out{ buildPath + std::string( "Main.js" ) };
		const auto& entityList = entityHand.ListEntities();

		out << "class Main\n{\n";

		out << "\tconstructor()\n\t{\n";
		// for( const auto& entity : entityList )
		// {
		// 	out << "\t\tthis." << entity << " = new " << entity << "()\n";
		// }
		out << "\t\tthis.entityList = [\n";
		for( const auto& entity : entityList )
		{
			out << "\t\t\tnew " << entity << "(),\n";
		}
		out << "\t\t]\n\n";
		out << "\t\tthis.CallStart()\n";
		out << "\t}\n";

		out << '\n';

		for( const auto& methodName : entityMethods )
		{
			const auto mName = std::string( methodName );
			out << "\tCall" + mName + "()\n\t{\n";

			// for( const auto& entity : entityList )
			// {
			// 	out << "\t\tthis." << entity << ".Entity" << mName << "()\n";
			// }
			out << "\t\tfor( let i = 0; i < this.entityList.length; ++i )\n";
			out << "\t\t{\n";
			for( const auto& entity : entityList )
			{
				out << "\t\t\tthis.entityList[i].Entity" << mName << "()\n";
			}
			out << "\t\t}\n";

			out << "\t}\n";

			out << '\n';
		}

		out << "}\n\n";

		out << "const main = new Main()\n";
		out << "const gfx = new Graphics()\n";
		// const entityList = main.entityList

		out << "setInterval( function()\n{\n";
		out << "\tgfx.DrawRect( 0,0,960,540,\"#000000\" )\n";
		out << "\tmain.CallUpdate()\n";
		out << "\tmain.CallDraw()\n";
		out << "},1000 / 30 )";
	}

	void WriteEntities( const EntityHandler& entityHand,
		const ModuleHandler& modHand )
	{
		for( const std::string& entity : entityHand.ListEntities() )
		{
			const auto& modList = entityHand.ShowEntity( entity );
			const auto& argList = entityHand.ShowEntityLong( entity );
			std::ofstream out{ buildPath + entity + ".js" };
			
			out << "class " << entity << " extends EntityBase\n{\n";
			out << "\tconstructor()\n\t{\n";
			out << "\t\tsuper()\n";
			// for( const std::string& mod : modList )
			for( int i = 0; i < int( modList.size() ); ++i )
			{
				const auto& mod = modList[i];
				out << "\t\tthis." << mod << " = new " <<
					entityHand.RemoveModNumber( mod );
				out << "( this,";
				// Args go here.
				for( int j = 1; j < int( argList[i].size() ); ++j )
				{
					const auto argVal = entityHand.RemoveArgName( argList[i][j] );
					bool isNumber = true;
					for( char c : argVal ) if( !isdigit( c ) && c != '.' ) isNumber = false;

					if( !isNumber ) out << '\"';
					out << argVal;
					if( !isNumber ) out << '\"';
					if( j < int( argList[i].size() ) - 1 ) out << ',';
				}
				out << " )\n";
			}
			out << "\t}\n";

			out << '\n';

			for( const auto& methodName : entityMethods )
			{
				const auto mName = std::string( methodName );
				out << "\tEntity" + mName + "()\n\t{\n";
				for( const auto& mod : modList )
				{
					out << "\t\tthis." << mod << ".On" + mName + "()\n";
				}
				out << "\t}\n";
				out << '\n';
			}

			out << "}";
		}
	}

	void WriteModules( const ModuleHandler& modHand )
	{
		for( const auto& modName : modHand.ListModules() )
		{
			std::ofstream out{ buildPath + modName + ".js" };

			for( const auto& line : modHand.ReadModuleCode( modName ) )
			{
				out << line << '\n';
			}
		}
	}

	void CopyFramework()
	{
		for( const auto& entry : std::filesystem::directory_iterator( frameworkPath ) )
		{
			const std::string file = entry.path().filename().string();
			std::filesystem::copy( entry,buildPath + file );
		}
	}

	void Cleanup()
	{
		std::filesystem::remove_all( buildPath );
		std::filesystem::create_directory( buildPath );
	}
private:
	static constexpr auto buildPath = "Build/";
	static constexpr auto frameworkPath = "Framework/";
	std::vector<std::string> entityMethods =
	{
		"Start",
		"Update",
		"Draw",
		"Collision",
		"Destroy"
	};
};
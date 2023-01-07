#include "files.h"
#include "../../dependencies/mocking_bird/mocking_bird.h"
#include <ShlObj_core.h>

std::filesystem::path files::impl::get_working_path( )
{
	std::filesystem::path path;

	MOCKING_TRY;

	if ( PWSTR documents = nullptr; SUCCEEDED( LI_FN( SHGetKnownFolderPath )( FOLDERID_Documents, 0, nullptr, &documents ) ) ) {
		path.assign( documents );
		path.append( _( ".hw" ) );
		CoTaskMemFree( documents ); // lazy importer crashes on this function
	}

	MOCKING_CATCH( return _( "" ) );

	return path;
}

void files::impl::refresh( )
{
	MOCKING_TRY;
	// clear out file name vector
	info.file_names.clear( );

	for ( const auto& i : std::filesystem::directory_iterator( fs_path ) ) {
		if ( i.path( ).filename( ).extension( ) == _( ".hw" ) ) {
			info.file_names.push_back( i.path( ).filename( ).string( ) );
		}
	}
	MOCKING_CATCH( return; );
}

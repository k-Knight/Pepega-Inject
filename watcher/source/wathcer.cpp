#include "../include/watcher.hpp"

using namespace efsw;
using namespace std;

Watcher::Watcher() { }

void Watcher::handleFileAction(WatchID watchid, const string& dir, const string& filename, Action action, string oldFilename) {
	switch( action ) {
	    case Actions::Add:
	    	
	    	break;
	    case Actions::Delete:
	    	
	    	break;
	    case Actions::Modified:
	    	
	    	break;
	    case Actions::Moved:
	    	
	    	break;
	    default:
            break;
	}
}
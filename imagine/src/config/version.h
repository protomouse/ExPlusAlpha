#pragma once

#define IMAGINE_VERSION_BASE "1.5.8"

#ifdef NDEBUG
	#define IMAGINE_VERSION IMAGINE_VERSION_BASE
#else
	#define IMAGINE_VERSION IMAGINE_VERSION_BASE "D"
#endif

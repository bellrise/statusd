/* Some defines of the network manager lib */

#ifndef NM_DEF_H
#define NM_DEF_H


enum NMConnectivityState
{
	NM_CONNECTIVITY_UNKNOWN = 0,
	NM_CONNECTIVITY_NONE = 1,
	NM_CONNECTIVITY_PORTAL = 2,
	NM_CONNECTIVITY_LIMITED = 3,
	NM_CONNECTIVITY_FULL = 4,
};


#endif /* NM_DEF_H */

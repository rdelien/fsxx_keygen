#ifndef ENCRYPTION_H
#define ENCRYPTION_H

/******************************************************************************/
/*** Global data                                                            ***/
/******************************************************************************/
extern const uint32_t      seeds[];
extern const unsigned int  nr_of_seeds;


/*****************************************************************************/
/*** Functions                                                             ***/
/*****************************************************************************/
uint32_t decrypt(const char *keyascii, uint32_t serialnr);
uint32_t encrypt(uint32_t seed, uint32_t serialnr);

#endif /* ENCRYPTION_H */

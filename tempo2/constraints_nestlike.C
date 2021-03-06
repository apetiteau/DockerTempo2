#include <cmath>
#include "constraints_nestlike.h"
#include <assert.h>
#include <string.h>
#define f1yr (1.0/3.16e7)

double constraints_nestlike_red(pulsar *psr,int ipsr, int iconstraint,int iparam,int constraintk,int k){
    assert(iconstraint == constraint_red_sin || iconstraint == constraint_red_cos);

    /* Constrain if
     *   This frequency matches the constraint frequency
     * and
     *   Sin/cos matches
     */
    if (constraintk==k &&
            (
             ((iconstraint == constraint_red_sin) && (iparam == param_red_sin)) ||
             ((iconstraint == constraint_red_cos) && (iparam == param_red_cos)) )
            ) {
        double maxtspan = psr[ipsr].param[param_finish].val[0] - psr[ipsr].param[param_start].val[0];
        double RedFLow = pow(10., psr[ipsr].TNRedFLow);
        double RedAmp = pow(10.,psr[ipsr].TNRedAmp);
        double freq = RedFLow*((double)(k+1.0))/(maxtspan);
        double RedIndex = psr[ipsr].TNRedGam;
        double RedCorner = psr[ipsr].TNRedCorner/maxtspan;

        /***
         * No idea what this equation represents! Copied from LL's code MJK2016
         */
        double rho = pow((1+(pow((1.0/365.25)/RedCorner,RedIndex/2))),2)*(RedAmp*RedAmp/12.0/(M_PI*M_PI))/pow((1+(pow(freq/RedCorner,RedIndex/2))),2)/(maxtspan*24*60*60)*pow(f1yr,-3.0);

        return 1.0/sqrt(rho);
    } else return 0;

}

double constraints_nestlike_jitter(pulsar *psr,int ipsr, int iconstraint,int iparam,int constraintk,int k){
    assert (iconstraint == constraint_jitter);

    if (iparam == param_jitter && constraintk == k) {
        double ecorrval=0;
        const int iobs = k;
        for (int iecorr=0; iecorr < psr[ipsr].nTNECORR; iecorr++){
            for (int iflag=0;iflag < psr[ipsr].obsn[iobs].nFlags; iflag++){
                if (
                        (strcmp(psr[ipsr].obsn[iobs].flagID[iflag],
                                psr[ipsr].TNECORRFlagID[iecorr])==0)
                        && (strcmp(psr[ipsr].obsn[iobs].flagVal[iflag],
                                psr[ipsr].TNECORRFlagVal[iecorr])==0)

                   ) {
                    ecorrval = psr[ipsr].TNECORRVal[iecorr];
                    break;
                }
                if (ecorrval > 0)break;
            }
        }
        return 1e6/ecorrval;
    } else {
        return 0;
    }
}

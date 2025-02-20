#ifndef REXBDD_REDUCTIONS_H
#define REXBDD_REDUCTIONS_H

#include "../defines.h"
#include "../error.h"

namespace REXBDD {
    ///  Reduction type
    enum ReductionType{
        QUASI,              // -+
        FULLY,              //  +---- only applicable if variable dimension is 1
        ZERO_SUP,           //  |
        REX,                // -+
        USER_DEFINED,       //        only if dimension is 1: user-defined combinations of reductions
        QUASI_QUASI,        // -+
        FULLY_FULLY,        //  +---- only applicable if variable dimension is 2
        IDENTITY_IDENTITY,  //  |
        FULLY_IDENTITY      // -+
    };
    static inline std::string reductionType2String(ReductionType rdt) {
        std::string redType;
        if (rdt == QUASI) {
            redType = "Quasi";
        } else if (rdt == FULLY) {
            redType = "Fully";
        } else if (rdt == ZERO_SUP) {
            redType = "Zero-Suppressed";
        } else if (rdt == REX) {
            redType = "Rex";
        } else if (rdt == USER_DEFINED) {
            redType = "User-Defined";
        } else if (rdt == QUASI_QUASI) {
            redType = "Quasi-Quasi";
        } else if (rdt == FULLY_FULLY) {
            redType = "Fully-Fully";
        } else if (rdt == IDENTITY_IDENTITY) {
            redType = "Identity-Identity";
        } else if (rdt == FULLY_IDENTITY) {
            redType = "Fully-Identity";
        } else {
            redType = "Unknown";
        }
        return redType;
    }
    //--------------------------+---------------------------+-----------------|
    // EL0    AL0 |  EL1    AL1 |  EH0    AH0 |  EH1    AH1 |  I0    X    I1
    //------------+-------------+-------------+-------------+-----------------|
    // 0      0   |  0      0   |  0      0   |  0      0   |  1     1    1
    // 0      0   |  0      0   |  1      1   |  1      1   |  0     0    0
    // 0      0   |  1      1   |  0      0   |  1      1   |  0     0    1
    // 0      1   |  0      1   |  0      1   |  0      1   |  0     1    0
    //------------+-------------+-------------+-------------+-----------------|
    // 0      1   |  2      3   |  4      5   |  6      7   |  8     9    10
    //--------------------------+---------------------------+-----------------|
    // Note:
    //      check X: rule == 1001; 
    //  otherwise
    //      check relation: rule > 7;
    //      check (?)1: rule & (0010);
    //  Complement rule:
    //      rule == X ? X :
    //          (rule & (0010)) ? (rule & (1101)) : (rule | (0010));
    //  Swap rule:
    //      rule >7 ? rule :
    //          (rule & (0010)) ? (rule & (1011)) : (rule | (0100));

    /// Reduction rule
    typedef enum {
        RULE_EL0 = 0,
        RULE_AL0 = 1,
        RULE_EL1 = 2,
        RULE_AL1 = 3,
        RULE_EH0 = 4,
        RULE_AH0 = 5,
        RULE_EH1 = 6,
        RULE_AH1 = 7,
        RULE_I0  = 8,
        RULE_X   = 9,
        RULE_I1  = 10
    } ReductionRule;
    static inline ReductionRule compRule(ReductionRule rule) {
        switch (rule) {
            case RULE_EL0: return RULE_EL1;
            case RULE_AL0: return RULE_AL1;
            case RULE_EL1: return RULE_EL0;
            case RULE_AL1: return RULE_AL0;
            case RULE_EH0: return RULE_EH1;
            case RULE_AH0: return RULE_AH1;
            case RULE_EH1: return RULE_EH0;
            case RULE_AH1: return RULE_AH0;
            case RULE_I0:  return RULE_I1;
            case RULE_X:   return RULE_X;
            case RULE_I1:  return RULE_I0;
            default: 
                std::cout << "[REXBDD] ERROR!\t Complement unknown reduction rule!" << std::endl;
                exit(0);
        }
    }
    static inline ReductionRule swapRule(ReductionRule rule) {
        switch (rule) {
            case RULE_EL0: return RULE_EH0;
            case RULE_AL0: return RULE_AH0;
            case RULE_EL1: return RULE_EH1;
            case RULE_AL1: return RULE_AH1;
            case RULE_EH0: return RULE_EL0;
            case RULE_AH0: return RULE_AL0;
            case RULE_EH1: return RULE_EL1;
            case RULE_AH1: return RULE_AL1;
            case RULE_I0:  return RULE_I0;  // swap-from_to
            case RULE_X:   return RULE_X;
            case RULE_I1:  return RULE_I1;  // swap-from_to
            default: 
                std::cout << "[REXBDD] ERROR!\t Complement unknown reduction rule!" << std::endl;
                exit(0);
        }
    }
    static inline bool hasRuleTerminalOne(ReductionRule rule) {
        return (bool)(rule & (0x01 << 1));
    }
    static inline bool isRuleEL(ReductionRule rule) {
        return (rule == RULE_EL0 || rule == RULE_EL1);
    }
    static inline bool isRuleEH(ReductionRule rule) {
        return (rule == RULE_EH0 || rule == RULE_EH1);
    }
    static inline bool isRuleAL(ReductionRule rule) {
        return (rule == RULE_AL0 || rule == RULE_AL1);
    }
    static inline bool isRuleAH(ReductionRule rule) {
        return (rule == RULE_AH0 || rule == RULE_AH1);
    }
    static inline bool isRuleI(ReductionRule rule) {
        return (rule == RULE_I0 || rule == RULE_I1);
    }
    static inline char rulePattern(ReductionRule rule) {
        char type;
        switch (rule) {
            case RULE_EL0:
            case RULE_EL1:
            case RULE_AH0:
            case RULE_AH1:
                type = 'L';
                break;
            case RULE_EH0:
            case RULE_EH1:
            case RULE_AL0:
            case RULE_AL1:
                type = 'H';
                break;
            default:
                type = 'U';
        }
        return type;
    }
    static inline std::string rule2String(ReductionRule rule) {
        switch (rule) {
            case RULE_EL0: return "EL0";
            case RULE_AL0: return "AL0";
            case RULE_EL1: return "EL1";
            case RULE_AL1: return "AL1";
            case RULE_EH0: return "EH0";
            case RULE_AH0: return "AH0";
            case RULE_EH1: return "EH1";
            case RULE_AH1: return "AH1";
            case RULE_I0:  return "I0";
            case RULE_X:   return "X";
            case RULE_I1:  return "I1";
            default: return "UNKNOWN_RULE";
        }
    }
    /// Reduction type and set of rules
    class Reductions;
}

// ******************************************************************
// *                                                                *
// *                                                                *
// *                         Reductions class                       *
// *                                                                *
// *                                                                *
// ******************************************************************
class REXBDD::Reductions {
    /*-------------------------------------------------------------*/
    public:
    /*-------------------------------------------------------------*/
        Reductions();
        Reductions(const ReductionType reductionType);
        Reductions(const std::vector<bool>& ruleSet);
        ~Reductions();
        //******************************************
        //  Getters
        //******************************************
        /* Dimension */
        /// Get the dimension
        inline int getDim() const {return dimension;}
        /// Get the type of reduction
        inline ReductionType getType() const {return type;}
        /// Get the size of reduction rules set
        inline int getNumRules() const {return std::count(rules.begin(),rules.end(), 1);}
        inline std::vector<bool> getRules() const {return rules; }
        /// Check if the given ReductionRule has been set
        inline bool hasRule(const ReductionRule rule) const {return rules[rule];}
        inline ReductionType rules2Type(const std::vector<bool>& ruleSet) const {
            ReductionType ans = USER_DEFINED;
            int numOnes = std::count(ruleSet.begin(),ruleSet.end(), 1);
            if (numOnes == 0) {
                // QUASI or QUASI_QUASI
                ans = (dimension>1)?QUASI_QUASI:QUASI;
            } else if (numOnes == 1 && ruleSet[RULE_X]) {
                // FULLY or FULLY_FULLY
                ans = (dimension>1)?FULLY_FULLY:FULLY;
            } else if (numOnes == 1 && ruleSet[RULE_EH0]) {
                // ZERO_SUP
                ans = ZERO_SUP;
            } else if (numOnes == 2 && ruleSet[RULE_I0] && ruleSet[RULE_I1]) {
                // IDENTITY_IDENTITY
                ans = IDENTITY_IDENTITY;
            } else if (numOnes == 3 && ruleSet[RULE_X] && ruleSet[RULE_I0] && ruleSet[RULE_I1]) {
                // FULLY_IDENTITY
                ans = FULLY_IDENTITY;
            } else if (numOnes == 9 && !ruleSet[RULE_I0] && !ruleSet[RULE_I1]) {
                // REX
                ans = REX;
            }
            return ans;
        }
        // more checkers? TBD
        //******************************************
        //  Setters
        //******************************************
        inline void setDim(const int dim) {
            if (dim > 2) {
                throw error(ErrCode::NOT_IMPLEMENTED, __FILE__, __LINE__);
                std::cout << "[REXBDD] Warning!\t Dimension "<<dim
                <<" not supported (not implemented yet)! Allowed: 1-2."<< std::endl;
            }
            dimension = dim;
        }
        inline void setType(const ReductionType reductionType) {
            dimension = 1;
            type = reductionType;
            // init all 0
            rules = std::vector<bool>(11,0);
            if (type == QUASI){
                // not change
            } else if (type == FULLY) {
                // Only apply reduction rule X
                rules[RULE_X] = 1;
            } else if (type == ZERO_SUP) {
                rules[RULE_EH0] = 1;
            } else if (type == REX) {
                // Apply all 9 rex reduction rules
                for (int i=0; i<11; i++) {
                    if ((ReductionRule)i != RULE_I0 && (ReductionRule)i != RULE_I1) rules[i] = 1;
                }
            } else if (type == QUASI_QUASI) {
                dimension = 2;
            } else if (type == FULLY_FULLY) {
                dimension = 2;
                // Only apply reduction rule X
                rules[RULE_X] = 1;
            } else if (type == IDENTITY_IDENTITY) {
                dimension = 2;
                rules[RULE_I0] = 1;
                // rules[RULE_I1] = 1;
            } else if (type == FULLY_IDENTITY) {
                dimension = 2;
                // Apply reduction rules: I0 (0), X (1)
                rules[RULE_X] = 1;
                rules[RULE_I0] = 1;
                rules[RULE_I1] = 1;
            } else if (type == USER_DEFINED) {
                // not change for user defined, it should be initialized later
            } else {
                // Error for Unknown
                type = USER_DEFINED;
                std::cout << "[REXBDD] Warning!\t Unknown reduction type, switched to User-defined type!"
                <<" Please check your input or add preferred rules by calling \"addReductionRule([PREFERRED RULE])\""<< std::endl;
                throw error(ErrCode::UNINITIALIZED, __FILE__, __LINE__);
            }
        }
        inline void addRule(const ReductionRule rule) {
            // This will automatically switch the type to USER_DEFINED, if rule is not allowed before
            if (rules[rule]) return;
            rules[rule] = 1;
            type = rules2Type(rules);   // update type
        }
        inline void delRule(const ReductionRule rule) {
            // This will automatically switch the type to USER_DEFINED, if rule is allowed before
            if (!rules[rule]) return;
            rules[rule] = 0;
            type = rules2Type(rules);   // update type
        }
        inline void setRules(const std::vector<bool>& ruleSet) {
            type = rules2Type(ruleSet);
            rules = ruleSet;
        }
        
    /*-------------------------------------------------------------*/
    private:
    /*-------------------------------------------------------------*/
        int                 dimension;  // 1: set/vector function; 2: relation/matrix function
        ReductionType       type;       // type of reductions
        std::vector<bool>   rules;      // set of allowed reduction rules
};


#endif
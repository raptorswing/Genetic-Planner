#ifndef MWCOMMAND_H
#define MWCOMMAND_H

class MWCommand
{
public:
    MWCommand();
    virtual ~MWCommand();

    virtual void exec()=0;
    virtual void unExec()=0;
};

#endif // MWCOMMAND_H

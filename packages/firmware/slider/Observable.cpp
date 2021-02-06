#include "inc/stdinc.h"

ObservableBase::ObservableBase()
{
    ObservableStore::sharedInstance().registerObservable(this);
}
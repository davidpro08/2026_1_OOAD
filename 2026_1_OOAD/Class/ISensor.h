#if !defined(_ISENSOR_H)
#define _ISENSOR_H


class ISensor {
public:
    virtual ~ISensor() {}
	virtual bool detect() = 0;
};

#endif  //_ISENSOR_H

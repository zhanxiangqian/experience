#ifndef IDIMENSIONCONVERT_H
#define IDIMENSIONCONVERT_H
#include <vector>
#include <map>
#include <QString>
#include <QStringList>

typedef struct Property
{
	double convert;
	unsigned char rank;
	QString name;
}Property;
typedef std::vector<Property> PropertyVector;
typedef std::map<QString, PropertyVector> UnitMap;

class IMaterialDimensionConvert
{
public: 
	virtual ~IMaterialDimensionConvert(){}

public:
	virtual double calculate(const QString& name, const QString& des, const QString& src) = 0;
	virtual void  setDimensionDocPath(const QString& path) = 0;
	virtual void readDimension() = 0; 
	virtual QStringList getUnitNameList(const QString & name ) = 0;
};
#endif //IDIMENSIONCONVERT_H

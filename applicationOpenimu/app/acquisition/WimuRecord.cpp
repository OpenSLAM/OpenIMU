#include"WimuRecord.h"
#include <QDebug>


WimuRecord::WimuRecord()
{

}
WimuRecord::~WimuRecord()
{

}
void WimuRecord::Serialize( Json::Value& root,RecordInfo infos,  std::string date,std::string& output )
{
    // serialize primitives
    //  root["_id"] = m_recordId;
    //  root["name"] = m_recordName;
}

void WimuRecord::Deserialize(Json::Value& root)
{
    // deserialize primitives
        //qDebug() << "here deserialize";
        for ( int index = 0; index < root.size(); ++index )
        {
            RecordInfo temp;
            temp.m_recordId = root[index].get("_id", "").asString();
            temp.m_recordName = root[index].get("name", "").asString();
            temp.m_imuType = root[index].get("format", "").asString();
            temp.m_imuPosition = root[index].get("position", "").asString();
            temp.m_recordDetails = root[index].get("comment", "").asString();
            m_WimuRecordList.push_back(temp);
        }
}

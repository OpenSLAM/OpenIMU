#include "TestBenchmark.h"

TestBenchmark::TestBenchmark(QObject *parent) : QObject(parent)
{
    caneva = new Caneva("../savefiles/test_block_mul.json",0);
}

void TestBenchmark::multiple_delay()
{
    QFETCH(std::vector<float> , input1);
    QFETCH(std::vector<float> , input2);
    QFETCH(std::vector<float> , result);

    std::vector<float> calculated;
    QBENCHMARK {
        caneva->getBlock("multiplier")->GetInput<float>("input1")->Put(input1);
        caneva->getBlock("multiplier")->GetInput<float>("input2")->Put(input2);
        calculated = caneva->getBlock("multiplier")->GetOutput<float>("output1")->getValueBuf();
    }

    unsigned int minSize = std::min(std::min(input1.size(),input2.size()), calculated.size());

    for(int i = 0; i<minSize; i++)
        qFuzzyCompare(calculated[i],result[i]);
}

void TestBenchmark::multiple_delay_data()
{
    QTest::addColumn<std::vector<float> >("input1");
    QTest::addColumn<std::vector<float> >("input2");
    QTest::addColumn<std::vector<float> >("result");

    QTest::newRow("3") << std::vector<float>({1.1f,2.2f,3.3f}) // input1
                       << std::vector<float>({1.1f,2.2f,3.3f}) // input2
                       << std::vector<float>({1.21f,4.84f,10.89f}); // output

    QTest::newRow("10") << std::vector<float>({1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f}) // input1
                       << std::vector<float>({1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f}) // input2
                       << std::vector<float>({1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.12f}); // output

    QTest::newRow("100") << std::vector<float>({1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f}) // input1
                       << std::vector<float>({1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f,2.2f,3.3f,1.1f}) // input2
                       << std::vector<float>({1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.12f,1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.12f,1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.12f,1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.12f,1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.12f,1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.12f,1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.12f,1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.12f,1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.12f,1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.21f,4.84f,10.89f,1.12f}); // output
}

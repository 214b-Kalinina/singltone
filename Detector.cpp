//Detector

class Detector
{
    protected:
    Detector(const std::string value): value_(value)
    {

    }

    static Detector* detector_;
    vstd::string value_;

    public:
    Detector(Detector &other) = delete;
    /**
    * Singletons should not be cloneable.
    */
    
    void operator=(const Detector &) = delete;
    /**
    * Singletons should not be assignable.
    */
    
    static Detector *GetInstance(const std::string& value);
    std::string value() const    {
        return value_;
     }
    
};

Detector* Detector::detector_= nullptr;

Detector *Detector::GetInstance(const std::string& value)
{
    if(detector_==nullptr){
        detector_ = new Detector(value);
    }
    return detector_;
}

void ThreadFoo(){
    // Following code emulates slow initialization.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Detector* detector = Detector::GetInstance("FOO");
    std::cout << detector->value() << "\n";
}

void ThreadBar(){
    // Following code emulates slow initialization.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Detector* detector = Detector::GetInstance("BAR");
    std::cout << detector->value() << "\n";
}

class FakeDetector : public Detector {
 public:
  std::string Operation() const override {
    return "{Result of the FakeDetector}";
  }
};
class MegaDetector : public Detector {
 public:
  std::string Operation() const override {
    return "{Result of the MegaDetector}";
  }
};


class Creator {

 public:
  virtual ~Creator(){};
  virtual Product* FactoryMethod() const = 0;

  std::string SomeOperation() const {
    // Call the factory method to create a Detector object.
    Detector* detector = this->FactoryMethod();
    // Now, use the product.
    std::string result = "Creator: The same creator's code has just worked with " + product->Operation();
    delete detector;
    return result;
  }
};

class ConcreteCreator1 : public Creator {
 public:
  Detector* FactoryMethod() const override {
    return new FakeDetector();
  }
};

class ConcreteCreator2 : public Creator {
 public:
  Detector* FactoryMethod() const override {
    return new MegaDetector();
  }
};


int main()
{
    std::cout <<"If you see the same value, then singleton was reused\n" <<
                "If you see different values, then 2 singletons were created\n\n" <<
                "RESULT:\n";   
    std::thread t1(ThreadFoo);
    std::thread t2(ThreadBar);
    t1.join();
    t2.join();

      std::cout << "App: Launched with the ConcreteCreator1.\n";
      Creator* creator = new ConcreteCreator1();
      std::cout << std::endl;
      std::cout << "App: Launched with the ConcreteCreator2.\n";
      Creator* creator2 = new ConcreteCreator2();

    delete creator;
    delete creator2;

    return 0;
}

# calculator

Калькулятор по дефолту поддерживает операции \*, +, /, - и скобки. Для загрузки через dll дополнительных операций, которые могут быть бинарные или унарные и как префиксные, так и постфиксные, нужно экспортировать там функцию, которая возвращает указатель на созданный объект класса, который удовлетворяют "интерфейсу":

    typedef void (*oper_t)(std::stack<double>&);

    class IOperation
    {
    public:
      virtual oper_t fun () = 0;
      virtual std::string name() = 0;
      virtual int priority() = 0;
      virtual bool isUnary() = 0;
      virtual bool isPrefixed() = 0;
      virtual ~IOperation() {};
    };

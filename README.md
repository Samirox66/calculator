# calculator

Калькулятор по дефолту поддерживает операции \*, +, /, - и скобки. Для загрузки через dll дополнительных операций, которые могут быть бинарные или унарные и как префиксные, так и постфиксные, нужно экспортировать там функцию, которая возвращает указатель на объект класса Operation:

    struct Operation
    {
      ~Operation() {}
      Operation(Operation const& other): isUnary(other.isUnary), name(other.name), priority(other.priority), oper(other.oper), isPrefixed(other.isPrefixed) {}

      bool operator < (Operation const& other) const
      {
        if (name == other.name)
        {
          return isUnary < other.isUnary;
        }

        return name < other.name;
      }

      Operation(oper_t oper, std::string name, int priority, bool isUnary, bool isPrefixed) : oper(oper), name(name), isUnary(isUnary), isPrefixed(isPrefixed), priority(priority) {}
      oper_t oper;
      bool isUnary;
      bool isPrefixed;
      std::string name;
      int priority;
    };

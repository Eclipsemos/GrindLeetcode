// 定义Pizza接口


/*
interface: pizza
    :getPrice();
class: basePizza implements pizza
abstract class implements pizza: pizzaDecorator 
class extends pizzaDecorator:CheeseTopping

interace: order
    :getPrice();
class: baseOrder implements order
abstract class: orderDecorator implements order
class extends orderDecorator: discountOrder();
*/
public interface Pizza {
    double getPrice();
}

// 实现基础Pizza类
public class BasePizza implements Pizza {
    private String base; // 底座类型
    private String size; // 尺寸

    public BasePizza(String base, String size) {
        this.base = base;
        this.size = size;
    }

    @Override
    public double getPrice() {
        // 根据base和size计算基础价格
        double price = 0;
        // 示例逻辑：这里只是伪代码，实际可以根据具体规则计算
        if ("thin".equals(base)) {
            price += 5;
        } else if ("thick".equals(base)) {
            price += 6;
        }
        if ("small".equals(size)) {
            price += 2;
        } else if ("large".equals(size)) {
            price += 4;
        }
        return price;
    }
}

// 定义抽象装饰器
public abstract class PizzaDecorator implements Pizza {
    protected Pizza pizza;

    public PizzaDecorator(Pizza pizza) {
        this.pizza = pizza;
    }
    
    @Override
    public double getPrice() {
        return pizza.getPrice();
    }
}

// 实现具体的Topping装饰器
public class CheeseTopping extends PizzaDecorator {
    public CheeseTopping(Pizza pizza) {
        super(pizza);
    }
    
    @Override
    public double getPrice() {
        // 加上奶酪的价格
        return super.getPrice() + 1.5;
    }
}

public class PepperoniTopping extends PizzaDecorator {
    public PepperoniTopping(Pizza pizza) {
        super(pizza);
    }
    
    @Override
    public double getPrice() {
        // 加上意大利香肠的价格
        return super.getPrice() + 2.0;
    }
}

// 其他Topping的装饰器可以类似实现

// 使用示例
public class PizzaStore {
    public static void main(String[] args) {
        // 创建一个基础Pizza，指定base和size
        Pizza myPizza = new BasePizza("thin", "large");
        
        // 添加奶酪
        myPizza = new CheeseTopping(myPizza);
        
        // 添加意大利香肠
        myPizza = new PepperoniTopping(myPizza);
        
        System.out.println("总价格: " + myPizza.getPrice());
    }
}

class MyClass:
    def __init__(self, num):
        self.num = num
    
    def user_functions(self):
        self.num = 100
        return f"{self.num}"

obj = MyClass(10)
obj.user_functions()
print(obj.num)
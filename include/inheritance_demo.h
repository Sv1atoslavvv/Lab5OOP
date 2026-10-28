#ifndef PROFSPILKA_LAB5_INHERITANCE_DEMO_H_
#define PROFSPILKA_LAB5_INHERITANCE_DEMO_H_

class LegacyBase {
 public:
  ~LegacyBase();  // Deliberately non-virtual for safe stack-lifetime contrast.
};

class LegacyDerived : public LegacyBase {
 public:
  ~LegacyDerived();
};

void demonstrate_polymorphism();
void demonstrate_destructor_rules();

#endif  // PROFSPILKA_LAB5_INHERITANCE_DEMO_H_

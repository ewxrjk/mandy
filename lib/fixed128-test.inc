static void fixed128_test_neg(void) {
  Fixed128 got;
  {
    const Fixed128 a = { .u64= { 0x1027c4d1c386bbc4, 0x00003c5f414c343c } };
    const Fixed128 expect = { .u64= { 0xefd83b2e3c79443c, 0xffffc3a0beb3cbc3 } };
    Fixed128_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xc2ce6f447ed4d57b, 0x0000f1ca7311d8a3 } };
    const Fixed128 expect = { .u64= { 0x3d3190bb812b2a85, 0xffff0e358cee275c } };
    Fixed128_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x612e7696a6cecc1b, 0x00006b7fc9e9c616 } };
    const Fixed128 expect = { .u64= { 0x9ed18969593133e5, 0xffff9480361639e9 } };
    Fixed128_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x9755d4c13a902931, 0x00003457f1fd42a2 } };
    const Fixed128 expect = { .u64= { 0x68aa2b3ec56fd6cf, 0xffffcba80e02bd5d } };
    Fixed128_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x51431193e6c3f339, 0x00000b6d07d4bedc } };
    const Fixed128 expect = { .u64= { 0xaebcee6c193c0cc7, 0xfffff492f82b4123 } };
    Fixed128_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xa648a7dd06839eb9, 0x000004b68a9a021e } };
    const Fixed128 expect = { .u64= { 0x59b75822f97c6147, 0xfffffb497565fde1 } };
    Fixed128_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xed2f89d94a2f20aa, 0x0000d51505805975 } };
    const Fixed128 expect = { .u64= { 0x12d07626b5d0df56, 0xffff2aeafa7fa68a } };
    Fixed128_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xb94067edfe175330, 0x000097c0dc2574bd } };
    const Fixed128 expect = { .u64= { 0x46bf981201e8acd0, 0xffff683f23da8b42 } };
    Fixed128_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xe901e35cd47d380d, 0x00006133ab99254a } };
    const Fixed128 expect = { .u64= { 0x16fe1ca32b82c7f3, 0xffff9ecc5466dab5 } };
    Fixed128_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x1ba1621582283d15, 0x000053d0c74803e3 } };
    const Fixed128 expect = { .u64= { 0xe45e9dea7dd7c2eb, 0xffffac2f38b7fc1c } };
    Fixed128_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xd707107e855c3844, 0x0000bdb564ac5db9 } };
    const Fixed128 expect = { .u64= { 0x28f8ef817aa3c7bc, 0xffff424a9b53a246 } };
    Fixed128_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xbb968a437d5c8dfc, 0x0000f04a07923986 } };
    const Fixed128 expect = { .u64= { 0x446975bc82a37204, 0xffff0fb5f86dc679 } };
    Fixed128_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xa5ac06d864c2f2e3, 0x000056512b9c014e } };
    const Fixed128 expect = { .u64= { 0x5a53f9279b3d0d1d, 0xffffa9aed463feb1 } };
    Fixed128_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x3a1890c78092b4d4, 0x0000064cfb695ffb } };
    const Fixed128 expect = { .u64= { 0xc5e76f387f6d4b2c, 0xfffff9b30496a004 } };
    Fixed128_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x8c5fe8f8dc3bf364, 0x0000cf143b6fe507 } };
    const Fixed128 expect = { .u64= { 0x73a0170723c40c9c, 0xffff30ebc4901af8 } };
    Fixed128_neg(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xbab9f87ff5059285, 0x0000c47201762741 } };
    const Fixed128 expect = { .u64= { 0x454607800afa6d7b, 0xffff3b8dfe89d8be } };
    Fixed128_neg(&got, &a);
    CHECK(got, expect);
  }
}

static void fixed128_test_square(void) {
  Fixed128 got;
  {
    const Fixed128 a = { .u64= { 0x177219d30e7a269f, 0x0000b8dc15ba2bdd } };
    const Fixed128 expect = { .u64= { 0x5bde966c725b35e1, 0x857d1c70f8e1484a } };
    Fixed128_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xda94e3e8ab73738f, 0x000080cf4ee207f8 } };
    const Fixed128 expect = { .u64= { 0x19857559471f2a72, 0x40cff6c2b1d04374 } };
    Fixed128_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x3653f8dd9b1f282e, 0x0000124b9b575bd1 } };
    const Fixed128 expect = { .u64= { 0x851dffb239f09b71, 0x014eb82cac5c1f38 } };
    Fixed128_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xdefc044a09325626, 0x0000ba600706a045 } };
    const Fixed128 expect = { .u64= { 0x2d0e72bd4659ee83, 0x87afae3ae60f195d } };
    Fixed128_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x2a1be9cd8697bbd0, 0x00005ad88f7d9b78 } };
    const Fixed128 expect = { .u64= { 0x204e17510d65606c, 0x203cfc16c5b469b4 } };
    Fixed128_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x3b08c6e33c729578, 0x00005a7b061b9030 } };
    const Fixed128 expect = { .u64= { 0xe46e0d89529f65af, 0x1ffabb6a4003c594 } };
    Fixed128_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x72154e76e4c11ab2, 0x0000d44fcbf87544 } };
    const Fixed128 expect = { .u64= { 0xaa5dce8de06c1840, 0xb01442b30815b84f } };
    Fixed128_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xa7a83ee0761ebfd2, 0x00007ff387c56473 } };
    const Fixed128 expect = { .u64= { 0xf4081dcfc2d188dc, 0x3ff38860e2693c0e } };
    Fixed128_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x47733e847d718d73, 0x0000ff02ecc1cb63 } };
    const Fixed128 expect = { .u64= { 0x0d5dbcd3d4d96bc9, 0xfe06d3b2a12cd798 } };
    Fixed128_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xa9643a295a9ac6de, 0x0000e8cce202849d } };
    const Fixed128 expect = { .u64= { 0x57663a2b6cbd7b8c, 0xd3b3fd9d8be99078 } };
    Fixed128_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xe73695c3e652c71a, 0x0000b3987604e4b4 } };
    const Fixed128 expect = { .u64= { 0x9d62142dbb8968ad, 0x7dfe8fd7340d1517 } };
    Fixed128_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x7ad1f45ae9500ec9, 0x00009b494f3f8777 } };
    const Fixed128 expect = { .u64= { 0xfbeeb5d33c23ead8, 0x5e31daf538ce363d } };
    Fixed128_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x9da59b74a6c3181c, 0x0000d03796838b76 } };
    const Fixed128 expect = { .u64= { 0xe1e3caee0861b064, 0xa95a60a7c7a5182b } };
    Fixed128_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xbb1e386c4fd5079e, 0x0000fa5135339774 } };
    const Fixed128 expect = { .u64= { 0x76eb3bd5ae9d805a, 0xf4c2b5ab79843c71 } };
    Fixed128_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x5769da05d205bbfc, 0x0000044fb9d7d01f } };
    const Fixed128 expect = { .u64= { 0xcc5380f2b8825583, 0x001296a2f89d0ecf } };
    Fixed128_square(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xc11e60de1b343f52, 0x000045e385b98f5f } };
    const Fixed128 expect = { .u64= { 0xe37250ae0884f3a1, 0x1314704c74383764 } };
    Fixed128_square(&got, &a);
    CHECK(got, expect);
  }
}

static void fixed128_test_sqrt(void) {
  Fixed128 got;
  {
    const Fixed128 a = { .u64= { 0x97b750923ceb3ffd, 0x000042c68b529b4a } };
    const Fixed128 expect = { .u64= { 0x44e0ef2df77abe12, 0x00000082bf00cd34 } };
    Fixed128_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xea7b5bf55eb561a4, 0x0000f2b79a9a80fd } };
    const Fixed128 expect = { .u64= { 0xd87f8fa5a0435d8b, 0x000000f945280e55 } };
    Fixed128_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xe8a8529f035efa25, 0x0000f03fd6645fa9 } };
    const Fixed128 expect = { .u64= { 0xfe4ffa7a20d72c76, 0x000000f7ffea8661 } };
    Fixed128_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x8d0038ec42650644, 0x0000622c3bfd1d33 } };
    const Fixed128 expect = { .u64= { 0xd3fd497bc682adc8, 0x0000009e880feff0 } };
    Fixed128_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x8cb4a0d7d6225675, 0x0000cb5579f248b0 } };
    const Fixed128 expect = { .u64= { 0x19b9c04b7839dd4e, 0x000000e426fdc23a } };
    Fixed128_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xdc6bf1e1a399f82a, 0x000076be268ecc45 } };
    const Fixed128 expect = { .u64= { 0x8caf534d033fb37f, 0x000000ae59c47915 } };
    Fixed128_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x63d2e49085ef3430, 0x000007c1bdc2ae99 } };
    const Fixed128 expect = { .u64= { 0x7cd0fd6863416110, 0x0000002c8fc8bc87 } };
    Fixed128_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xc6f8da3eabe19f58, 0x0000519c10645d51 } };
    const Fixed128 expect = { .u64= { 0xc3878668ae80823c, 0x000000908a76aada } };
    Fixed128_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xf51e8722c21b6092, 0x000015e897524d6a } };
    const Fixed128 expect = { .u64= { 0x2c5a14329f5258cd, 0x0000004ae3f99b24 } };
    Fixed128_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xeb8f624fb804d820, 0x0000c674e0f9e038 } };
    const Fixed128 expect = { .u64= { 0x340bc95b65fc8df8, 0x000000e1663d8592 } };
    Fixed128_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xc9c18070b6d13089, 0x0000da97ebcd1f5e } };
    const Fixed128 expect = { .u64= { 0x4dd57aa42fd8953e, 0x000000ec8ef97702 } };
    Fixed128_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xf6ced90a71d2af72, 0x000044afef829c88 } };
    const Fixed128 expect = { .u64= { 0x9a56ff07d78307a9, 0x000000849abc1aea } };
    Fixed128_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x5d92b243e0fd67dd, 0x0000125f18f2c41c } };
    const Fixed128 expect = { .u64= { 0xd5b54cb94fd8f750, 0x0000004494420beb } };
    Fixed128_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x7eb0adf422cedafb, 0x00008416378c74dc } };
    const Fixed128 expect = { .u64= { 0xd05db20186585d5f, 0x000000b7e2ecb171 } };
    Fixed128_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xdaf0105ba06c05a1, 0x0000d7a04d100d8f } };
    const Fixed128 expect = { .u64= { 0x0a37482fa7b1053d, 0x000000eaf28ba1c9 } };
    Fixed128_sqrt(&got, &a);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x88bafad959d54505, 0x0000d0ad95c76ab4 } };
    const Fixed128 expect = { .u64= { 0x6cd8a64bfb60e2b8, 0x000000e7218fb9a7 } };
    Fixed128_sqrt(&got, &a);
    CHECK(got, expect);
  }
}

static void fixed128_test_add(void) {
  Fixed128 got;
  {
    const Fixed128 a = { .u64= { 0x1027c4d1c386bbc4, 0x00003c5f414c343c } };
    const Fixed128 b = { .u64= { 0xc2ce6f447ed4d57b, 0x0000f1ca7311d8a3 } };
    const Fixed128 expect = { .u64= { 0xd2f63416425b913f, 0x00012e29b45e0cdf } };
    Fixed128_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x612e7696a6cecc1b, 0x00006b7fc9e9c616 } };
    const Fixed128 b = { .u64= { 0x9755d4c13a902931, 0x00003457f1fd42a2 } };
    const Fixed128 expect = { .u64= { 0xf8844b57e15ef54c, 0x00009fd7bbe708b8 } };
    Fixed128_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x51431193e6c3f339, 0x00000b6d07d4bedc } };
    const Fixed128 b = { .u64= { 0xa648a7dd06839eb9, 0x000004b68a9a021e } };
    const Fixed128 expect = { .u64= { 0xf78bb970ed4791f2, 0x00001023926ec0fa } };
    Fixed128_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xed2f89d94a2f20aa, 0x0000d51505805975 } };
    const Fixed128 b = { .u64= { 0xb94067edfe175330, 0x000097c0dc2574bd } };
    const Fixed128 expect = { .u64= { 0xa66ff1c7484673da, 0x00016cd5e1a5ce33 } };
    Fixed128_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xe901e35cd47d380d, 0x00006133ab99254a } };
    const Fixed128 b = { .u64= { 0x1ba1621582283d15, 0x000053d0c74803e3 } };
    const Fixed128 expect = { .u64= { 0x04a3457256a57522, 0x0000b50472e1292e } };
    Fixed128_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xd707107e855c3844, 0x0000bdb564ac5db9 } };
    const Fixed128 b = { .u64= { 0xbb968a437d5c8dfc, 0x0000f04a07923986 } };
    const Fixed128 expect = { .u64= { 0x929d9ac202b8c640, 0x0001adff6c3e9740 } };
    Fixed128_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xa5ac06d864c2f2e3, 0x000056512b9c014e } };
    const Fixed128 b = { .u64= { 0x3a1890c78092b4d4, 0x0000064cfb695ffb } };
    const Fixed128 expect = { .u64= { 0xdfc4979fe555a7b7, 0x00005c9e27056149 } };
    Fixed128_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x8c5fe8f8dc3bf364, 0x0000cf143b6fe507 } };
    const Fixed128 b = { .u64= { 0xbab9f87ff5059285, 0x0000c47201762741 } };
    const Fixed128 expect = { .u64= { 0x4719e178d14185e9, 0x000193863ce60c49 } };
    Fixed128_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x7b297d0b0e5e18ba, 0x0000babedeb8fc4c } };
    const Fixed128 b = { .u64= { 0x6a17b9af5b569643, 0x000000cf58989008 } };
    const Fixed128 expect = { .u64= { 0xe54136ba69b4aefd, 0x0000bb8e37518c54 } };
    Fixed128_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x8cfe5cd12d5db79b, 0x00005c8f959f3a51 } };
    const Fixed128 b = { .u64= { 0x47fc816ac16e2284, 0x0000898b3fe31d03 } };
    const Fixed128 expect = { .u64= { 0xd4fade3beecbda1f, 0x0000e61ad5825754 } };
    Fixed128_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xcc1b0c3e1c07724e, 0x00005e859ff3078f } };
    const Fixed128 b = { .u64= { 0x4a5012dc582c18c9, 0x000055be11cbc288 } };
    const Fixed128 expect = { .u64= { 0x166b1f1a74338b17, 0x0000b443b1beca18 } };
    Fixed128_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x7f1a355e526eb523, 0x00003a7779490eab } };
    const Fixed128 b = { .u64= { 0x4fdf8e1a060cea63, 0x0000afca62f5680c } };
    const Fixed128 expect = { .u64= { 0xcef9c378587b9f86, 0x0000ea41dc3e76b7 } };
    Fixed128_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xcbd3f5e06bc15385, 0x0000844f3023580c } };
    const Fixed128 b = { .u64= { 0x39b21c95055455e8, 0x0000cb6c0492c4f5 } };
    const Fixed128 expect = { .u64= { 0x058612757115a96d, 0x00014fbb34b61d02 } };
    Fixed128_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xb1eedaffcc3d5506, 0x0000e6d5843fdda7 } };
    const Fixed128 b = { .u64= { 0x861e02ec39235bc0, 0x00000fb7a6048457 } };
    const Fixed128 expect = { .u64= { 0x380cddec0560b0c6, 0x0000f68d2a4461ff } };
    Fixed128_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xa8ea37f7523d2a54, 0x0000da43a185cc8e } };
    const Fixed128 b = { .u64= { 0xbcc99ae80f0c8a89, 0x000040594c717095 } };
    const Fixed128 expect = { .u64= { 0x65b3d2df6149b4dd, 0x00011a9cedf73d24 } };
    Fixed128_add(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x364e433ff7c882f4, 0x0000184ae023033d } };
    const Fixed128 b = { .u64= { 0x121b28004e6f5a94, 0x00002723dbc799b0 } };
    const Fixed128 expect = { .u64= { 0x48696b404637dd88, 0x00003f6ebbea9ced } };
    Fixed128_add(&got, &a, &b);
    CHECK(got, expect);
  }
}

static void fixed128_test_sub(void) {
  Fixed128 got;
  {
    const Fixed128 a = { .u64= { 0x177219d30e7a269f, 0x0000b8dc15ba2bdd } };
    const Fixed128 b = { .u64= { 0xda94e3e8ab73738f, 0x000080cf4ee207f8 } };
    const Fixed128 expect = { .u64= { 0x3cdd35ea6306b310, 0x0000380cc6d823e4 } };
    Fixed128_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x3653f8dd9b1f282e, 0x0000124b9b575bd1 } };
    const Fixed128 b = { .u64= { 0xdefc044a09325626, 0x0000ba600706a045 } };
    const Fixed128 expect = { .u64= { 0x5757f49391ecd208, 0xffff57eb9450bb8b } };
    Fixed128_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x2a1be9cd8697bbd0, 0x00005ad88f7d9b78 } };
    const Fixed128 b = { .u64= { 0x3b08c6e33c729578, 0x00005a7b061b9030 } };
    const Fixed128 expect = { .u64= { 0xef1322ea4a252658, 0x0000005d89620b47 } };
    Fixed128_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x72154e76e4c11ab2, 0x0000d44fcbf87544 } };
    const Fixed128 b = { .u64= { 0xa7a83ee0761ebfd2, 0x00007ff387c56473 } };
    const Fixed128 expect = { .u64= { 0xca6d0f966ea25ae0, 0x0000545c443310d0 } };
    Fixed128_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x47733e847d718d73, 0x0000ff02ecc1cb63 } };
    const Fixed128 b = { .u64= { 0xa9643a295a9ac6de, 0x0000e8cce202849d } };
    const Fixed128 expect = { .u64= { 0x9e0f045b22d6c695, 0x000016360abf46c5 } };
    Fixed128_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xe73695c3e652c71a, 0x0000b3987604e4b4 } };
    const Fixed128 b = { .u64= { 0x7ad1f45ae9500ec9, 0x00009b494f3f8777 } };
    const Fixed128 expect = { .u64= { 0x6c64a168fd02b851, 0x0000184f26c55d3d } };
    Fixed128_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x9da59b74a6c3181c, 0x0000d03796838b76 } };
    const Fixed128 b = { .u64= { 0xbb1e386c4fd5079e, 0x0000fa5135339774 } };
    const Fixed128 expect = { .u64= { 0xe287630856ee107e, 0xffffd5e6614ff401 } };
    Fixed128_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x5769da05d205bbfc, 0x0000044fb9d7d01f } };
    const Fixed128 b = { .u64= { 0xc11e60de1b343f52, 0x000045e385b98f5f } };
    const Fixed128 expect = { .u64= { 0x964b7927b6d17caa, 0xffffbe6c341e40bf } };
    Fixed128_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xf18e859835e1f291, 0x00001eeae16dce72 } };
    const Fixed128 b = { .u64= { 0x0e8a35b10828d569, 0x0000b86e5cc36c27 } };
    const Fixed128 expect = { .u64= { 0xe3044fe72db91d28, 0xffff667c84aa624b } };
    Fixed128_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x3fdf57cd2c006497, 0x00000c00ac3a5b26 } };
    const Fixed128 b = { .u64= { 0x1d7f427515392480, 0x0000228af45e2fa0 } };
    const Fixed128 expect = { .u64= { 0x2260155816c74017, 0xffffe975b7dc2b86 } };
    Fixed128_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x5f834da70569c018, 0x0000416d4174e75a } };
    const Fixed128 b = { .u64= { 0x85e951862f0981ae, 0x000000fdb105d83e } };
    const Fixed128 expect = { .u64= { 0xd999fc20d6603e6a, 0x0000406f906f0f1b } };
    Fixed128_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x49390aa51cf5192b, 0x0000fa3e5653a45c } };
    const Fixed128 b = { .u64= { 0x9aeb7fa0c4169b14, 0x0000176fbd6d2500 } };
    const Fixed128 expect = { .u64= { 0xae4d8b0458de7e17, 0x0000e2ce98e67f5b } };
    Fixed128_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x4391b6e2e6eacb0f, 0x0000cdbfc1731339 } };
    const Fixed128 b = { .u64= { 0x9f20dbb0dcc93f0e, 0x00004e91b4917fc0 } };
    const Fixed128 expect = { .u64= { 0xa470db320a218c01, 0x00007f2e0ce19378 } };
    Fixed128_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xf572df00790813e3, 0x00002fd939bc2ccd } };
    const Fixed128 b = { .u64= { 0xc7f3860895bfa813, 0x0000f94d649889c0 } };
    const Fixed128 expect = { .u64= { 0x2d7f58f7e3486bd0, 0xffff368bd523a30d } };
    Fixed128_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x574f1eedf5b0f16c, 0x0000860f425424a1 } };
    const Fixed128 b = { .u64= { 0xaba9e7b823fc5ad2, 0x000081840e893302 } };
    const Fixed128 expect = { .u64= { 0xaba53735d1b4969a, 0x0000048b33caf19e } };
    Fixed128_sub(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x21b7379f0897246a, 0x0000576729421c40 } };
    const Fixed128 b = { .u64= { 0x74115c86188b1044, 0x0000769ba294523d } };
    const Fixed128 expect = { .u64= { 0xada5db18f00c1426, 0xffffe0cb86adca02 } };
    Fixed128_sub(&got, &a, &b);
    CHECK(got, expect);
  }
}

static void fixed128_test_mul(void) {
  Fixed128 got;
  {
    const Fixed128 a = { .u64= { 0x97b750923ceb3ffd, 0x000042c68b529b4a } };
    const Fixed128 b = { .u64= { 0xea7b5bf55eb561a4, 0x0000f2b79a9a80fd } };
    const Fixed128 expect = { .u64= { 0xaf5c2c52ad81404d, 0x3f4f93f56bbe87e4 } };
    Fixed128_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xe8a8529f035efa25, 0x0000f03fd6645fa9 } };
    const Fixed128 b = { .u64= { 0x8d0038ec42650644, 0x0000622c3bfd1d33 } };
    const Fixed128 expect = { .u64= { 0x936f12f71ad7b137, 0x5c21f35786c71bd7 } };
    Fixed128_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x8cb4a0d7d6225675, 0x0000cb5579f248b0 } };
    const Fixed128 b = { .u64= { 0xdc6bf1e1a399f82a, 0x000076be268ecc45 } };
    const Fixed128 expect = { .u64= { 0x58e2df46f6a0c320, 0x5e506e464b16895c } };
    Fixed128_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x63d2e49085ef3430, 0x000007c1bdc2ae99 } };
    const Fixed128 b = { .u64= { 0xc6f8da3eabe19f58, 0x0000519c10645d51 } };
    const Fixed128 expect = { .u64= { 0x80157045965c9fc4, 0x02790799623f1548 } };
    Fixed128_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xf51e8722c21b6092, 0x000015e897524d6a } };
    const Fixed128 b = { .u64= { 0xeb8f624fb804d820, 0x0000c674e0f9e038 } };
    const Fixed128 expect = { .u64= { 0x77dab52f895ab25b, 0x10fbe5af1fff6449 } };
    Fixed128_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xc9c18070b6d13089, 0x0000da97ebcd1f5e } };
    const Fixed128 b = { .u64= { 0xf6ced90a71d2af72, 0x000044afef829c88 } };
    const Fixed128 expect = { .u64= { 0x9e23d34b7624a84a, 0x3aa69500098164fb } };
    Fixed128_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x5d92b243e0fd67dd, 0x0000125f18f2c41c } };
    const Fixed128 b = { .u64= { 0x7eb0adf422cedafb, 0x00008416378c74dc } };
    const Fixed128 expect = { .u64= { 0xf3c5d1910da83feb, 0x097aa105d4bc31b3 } };
    Fixed128_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xdaf0105ba06c05a1, 0x0000d7a04d100d8f } };
    const Fixed128 b = { .u64= { 0x88bafad959d54505, 0x0000d0ad95c76ab4 } };
    const Fixed128 expect = { .u64= { 0xc25fa6de34b855ba, 0xafc474196334bb42 } };
    Fixed128_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x3b7dae0495918694, 0x0000ac6ce779c470 } };
    const Fixed128 b = { .u64= { 0xea6d5547ae966193, 0x00000eacedcf6109 } };
    const Fixed128 expect = { .u64= { 0x0be35b8747afa818, 0x09e26e00ae44f669 } };
    Fixed128_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x926baeafe79a27e6, 0x0000354991b1078e } };
    const Fixed128 b = { .u64= { 0xfcf249f3d4e441c3, 0x000088bf92d3043a } };
    const Fixed128 expect = { .u64= { 0x9dbd89ba8c5ef5ae, 0x1c76f5d9a0b3d9f9 } };
    Fixed128_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x1fdaf62548f2f8ed, 0x0000f6c8103ef3c2 } };
    const Fixed128 b = { .u64= { 0xa385ac4bda9bf98c, 0x0000f78efd63ed5b } };
    const Fixed128 expect = { .u64= { 0x58eba6acfd0de5e1, 0xeea4dee9cc578f73 } };
    Fixed128_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x5815a3d516a91f39, 0x0000221accf3d0b3 } };
    const Fixed128 b = { .u64= { 0xe5a8181b691406be, 0x00000a4d26988f4f } };
    const Fixed128 expect = { .u64= { 0xaf6c766d1efdb4e2, 0x015f533578ea467a } };
    Fixed128_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x6d59298c4b3c74f7, 0x0000d494c4cf8b96 } };
    const Fixed128 b = { .u64= { 0xc2fa7b1f9d5200ef, 0x0000c16f0b813439 } };
    const Fixed128 expect = { .u64= { 0x26b78fa5d1b58265, 0xa0a05e6f7d043541 } };
    Fixed128_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xec007b1be1830294, 0x00008ee2fb0af1e3 } };
    const Fixed128 b = { .u64= { 0x3c67523f81633acf, 0x00001270ff9e4840 } };
    const Fixed128 expect = { .u64= { 0x507440eb8d5ffed6, 0x0a4b07a10a466313 } };
    Fixed128_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x01da01354f468977, 0x0000375b13b45a39 } };
    const Fixed128 b = { .u64= { 0xf8af8c793287d050, 0x0000954e686e80a9 } };
    const Fixed128 expect = { .u64= { 0xc045058c9c678299, 0x2048f6ccde6cb258 } };
    Fixed128_mul(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xfad9d3a90add12e3, 0x0000adfcde26c45b } };
    const Fixed128 b = { .u64= { 0x5c35d7ed5057326c, 0x000046d2f56ab44e } };
    const Fixed128 expect = { .u64= { 0x158944ab95b7a290, 0x302284f85b585def } };
    Fixed128_mul(&got, &a, &b);
    CHECK(got, expect);
  }
}

static void fixed128_test_div(void) {
  Fixed128 got;
  {
    const Fixed128 a = { .u64= { 0x7a97c643656412a9, 0x00002e2127ac435a } };
    const Fixed128 b = { .u64= { 0xd3addccb2c33be0a, 0x000086001b2ed40e } };
    const Fixed128 expect = { .u64= { 0x2cd7c3d8531bd206, 0x000000005820b42f } };
    Fixed128_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xf165c8ce36e2f24b, 0x00000d20ed6f0b09 } };
    const Fixed128 b = { .u64= { 0xa4042bb3d4341aad, 0x00008540ce80c4b0 } };
    const Fixed128 expect = { .u64= { 0x91286093778b2132, 0x000000001938dd1b } };
    Fixed128_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x459142deccea2645, 0x000054633184ff27 } };
    const Fixed128 b = { .u64= { 0x566002249b191bf4, 0x0000c69fabf4a07c } };
    const Fixed128 expect = { .u64= { 0xfaa82a8cdf800187, 0x000000006cc3b106 } };
    Fixed128_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x3fb62d2c81862fc9, 0x00007ea12d83a823 } };
    const Fixed128 b = { .u64= { 0xd7288ff68c320f89, 0x000003b14cdce7a6 } };
    const Fixed128 expect = { .u64= { 0x4357322b40789298, 0x000000224b01cd83 } };
    Fixed128_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x31f3b9238224b122, 0x0000d8f369f85e31 } };
    const Fixed128 b = { .u64= { 0x49c7b59b995253fd, 0x0000e71a6e58d5ca } };
    const Fixed128 expect = { .u64= { 0x866ce096e24b1843, 0x00000000f052acdd } };
    Fixed128_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x3bb4a570294c4ea3, 0x000084f14e1bcb38 } };
    const Fixed128 b = { .u64= { 0xcc21ce88d0060cc5, 0x000029820b11ad28 } };
    const Fixed128 expect = { .u64= { 0x13cb40c49f7b7267, 0x0000000333ec0503 } };
    Fixed128_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xb36cc9aa78a330a1, 0x00004a4557c49391 } };
    const Fixed128 b = { .u64= { 0xac7cc4a4ff4dab10, 0x0000220432111ac1 } };
    const Fixed128 expect = { .u64= { 0x2e12ec215a0c8aec, 0x000000022ef25050 } };
    Fixed128_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x70ef55b1a1f65507, 0x00005e0e46bbe9e8 } };
    const Fixed128 b = { .u64= { 0xa26a25c852175b7a, 0x000065bd8efbc170 } };
    const Fixed128 expect = { .u64= { 0x073910b02be6483e, 0x00000000eca9e976 } };
    Fixed128_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xb54a23020fc5b043, 0x00008e113a973000 } };
    const Fixed128 b = { .u64= { 0x4a7a03052d733dcd, 0x00000d25758240df } };
    const Fixed128 expect = { .u64= { 0x4c45c587e76ed5d4, 0x0000000ace7e7eba } };
    Fixed128_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x5b69dc230af5ac87, 0x00002a4bb281b888 } };
    const Fixed128 b = { .u64= { 0xf6e7d078e55b85dd, 0x00009245f469aef8 } };
    const Fixed128 expect = { .u64= { 0x7ddfc331f66946b1, 0x000000004a061561 } };
    Fixed128_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xacdac615bc20f626, 0x0000a77cf5b9e1f5 } };
    const Fixed128 b = { .u64= { 0x52a3b18104a7f007, 0x0000a4b249fbac36 } };
    const Fixed128 expect = { .u64= { 0x6a949c99e5a5fc49, 0x000000010456ddcc } };
    Fixed128_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xae17584a9ed9c621, 0x000027cfd163b764 } };
    const Fixed128 b = { .u64= { 0x4ac034cf71b34e47, 0x0000800622ed9387 } };
    const Fixed128 expect = { .u64= { 0x73c959eebce6e37c, 0x000000004f9bd1c0 } };
    Fixed128_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x994b971761b2ceba, 0x0000515bf6396ae3 } };
    const Fixed128 b = { .u64= { 0x92b607d554d08ce6, 0x0000ba050267ce8c } };
    const Fixed128 expect = { .u64= { 0xdb0cf193a45ed63c, 0x000000006ff76d32 } };
    Fixed128_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x746ccfcd0b77d43a, 0x0000bafa2b68beef } };
    const Fixed128 b = { .u64= { 0x924aecbe4a53583b, 0x0000e0ef18dbb0c1 } };
    const Fixed128 expect = { .u64= { 0x866e905e2a19ded8, 0x00000000d4cd0ced } };
    Fixed128_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0x1d1615ad353a09cf, 0x00001fd90f30e005 } };
    const Fixed128 b = { .u64= { 0x52496e1e3fc24ec0, 0x00001236ff651b90 } };
    const Fixed128 expect = { .u64= { 0x0be9987abafaa1c8, 0x00000001bf9aff45 } };
    Fixed128_div(&got, &a, &b);
    CHECK(got, expect);
  }
  {
    const Fixed128 a = { .u64= { 0xd5b8c2551f4d4cc5, 0x000095f7877c606f } };
    const Fixed128 b = { .u64= { 0x33445533fcd71d42, 0x0000674e7a4e9ba3 } };
    const Fixed128 expect = { .u64= { 0xa0089a491459977c, 0x0000000173a092ab } };
    Fixed128_div(&got, &a, &b);
    CHECK(got, expect);
  }
}

static void fixed128_test_compare(void) {
  bool got;
  {
    const Fixed128 a = { .u64= { 0x0000000000000000, 0x8000000000000000 } };
    const Fixed128 b = { .u64= { 0x0000000000000000, 0x8000000000000000 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const Fixed128 a = { .u64= { 0x0000000000000000, 0x8000000000000000 } };
    const Fixed128 b = { .u64= { 0x0000000000000000, 0x0000000100000000 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x0000000000000000, 0x8000000000000000 } };
    const Fixed128 b = { .u64= { 0x0000000000000000, 0x0000000000000000 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x0000000000000000, 0x8000000000000000 } };
    const Fixed128 b = { .u64= { 0xffffffffffffffff, 0x7fffffffffffffff } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x0000000000000000, 0x0000000100000000 } };
    const Fixed128 b = { .u64= { 0x0000000000000000, 0x8000000000000000 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x0000000000000000, 0x0000000100000000 } };
    const Fixed128 b = { .u64= { 0x0000000000000000, 0x0000000100000000 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const Fixed128 a = { .u64= { 0x0000000000000000, 0x0000000100000000 } };
    const Fixed128 b = { .u64= { 0x0000000000000000, 0x0000000000000000 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x0000000000000000, 0x0000000100000000 } };
    const Fixed128 b = { .u64= { 0xffffffffffffffff, 0x7fffffffffffffff } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x0000000000000000, 0x0000000000000000 } };
    const Fixed128 b = { .u64= { 0x0000000000000000, 0x8000000000000000 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x0000000000000000, 0x0000000000000000 } };
    const Fixed128 b = { .u64= { 0x0000000000000000, 0x0000000100000000 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x0000000000000000, 0x0000000000000000 } };
    const Fixed128 b = { .u64= { 0x0000000000000000, 0x0000000000000000 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const Fixed128 a = { .u64= { 0x0000000000000000, 0x0000000000000000 } };
    const Fixed128 b = { .u64= { 0xffffffffffffffff, 0x7fffffffffffffff } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0xffffffffffffffff, 0x7fffffffffffffff } };
    const Fixed128 b = { .u64= { 0x0000000000000000, 0x8000000000000000 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0xffffffffffffffff, 0x7fffffffffffffff } };
    const Fixed128 b = { .u64= { 0x0000000000000000, 0x0000000100000000 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0xffffffffffffffff, 0x7fffffffffffffff } };
    const Fixed128 b = { .u64= { 0x0000000000000000, 0x0000000000000000 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0xffffffffffffffff, 0x7fffffffffffffff } };
    const Fixed128 b = { .u64= { 0xffffffffffffffff, 0x7fffffffffffffff } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const Fixed128 a = { .u64= { 0x4da4f9fc3c6da5d7, 0xb8a1abcd1a6916c7 } };
    const Fixed128 b = { .u64= { 0x4da4f9fc3c6da5d7, 0xb8a1abcd1a6916c7 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const Fixed128 a = { .u64= { 0x27ac435a7a97c643, 0x110722311710cf53 } };
    const Fixed128 b = { .u64= { 0x27ac435a7a97c643, 0x110722311710cf53 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const Fixed128 a = { .u64= { 0x5c3902b38963dc6e, 0xc79d679346d4ac7a } };
    const Fixed128 b = { .u64= { 0xccea264542a00403, 0x3184ff27459142de } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0xdaca3c06f5ff0c03, 0xe14b0190d93936e1 } };
    const Fixed128 b = { .u64= { 0x81862fc9634f806f, 0x2d83a8233fb62d2c } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x6e58d5ca49c7b59b, 0x294c4ea3738d243a } };
    const Fixed128 b = { .u64= { 0x4278c2614e1bcb38, 0xcc21ce88d0060cc5 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x84d4cd1f47ca7883, 0xa5e333cb88dcf943 } };
    const Fixed128 b = { .u64= { 0x11021c9e32111ac1, 0xe9dd38b869ace913 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0xa1f65507a2909cb6, 0x46bbe9e870ef55b1 } };
    const Fixed128 b = { .u64= { 0x6f98bca35b17b966, 0x96b98b5fbf37a2be } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x8efbc170a26a25c8, 0xe78131c132decd6b } };
    const Fixed128 b = { .u64= { 0xd6e4a51519d9c9cc, 0xb54a23020fc5b043 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x1f44ebd13cc75f3e, 0xef40af2e54c0ce68 } };
    const Fixed128 b = { .u64= { 0x758240df4a7a0305, 0x0af5ac870692b534 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0xbc20f6264922b9cc, 0xf5b9e1f5acdac615 } };
    const Fixed128 b = { .u64= { 0x4ac034cf71b34e47, 0x40031ad622ed9387 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x5d02db430267ce8c, 0x746ccfcd0b77d43a } };
    const Fixed128 b = { .u64= { 0x6c8826ec350d775d, 0x353a09cfeaa1b295 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x0fecf10e0f30e005, 0xbcb3fd500e263730 } };
    const Fixed128 b = { .u64= { 0x8bd5bb710a77ec0c, 0x952989c17d9c649a } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x4afbfae4877c606f, 0xff353728c6173d94 } };
    const Fixed128 b = { .u64= { 0xfcd71d42a6d00e34, 0x7a4e9ba333445533 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x705620733deaaddd, 0x7defb12b691e8e3b } };
    const Fixed128 b = { .u64= { 0x6d86b88de3a9312c, 0x37396957d4bf8115 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x081a3cfe300dc4c2, 0x411ff179096c1dbb } };
    const Fixed128 b = { .u64= { 0x8692c6f33e0d36b7, 0xc5b679993543c7a6 } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0xdea5486a6ac9573d, 0x2446913842fdef77 } };
    const Fixed128 b = { .u64= { 0xbea661c3b7a46957, 0x7e89a8ed0a5e6bea } };
    got = Fixed128_lt(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_gt(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_eq(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ne(&a, &b);
    CHECK_BOOL(got, true);
  }
}

static void fixed128_test_compare_unsigned(void) {
  bool got;
  {
    const Fixed128 a = { .u64= { 0xe656cae2928291e0, 0xecb11a5af1b31705 } };
    const Fixed128 b = { .u64= { 0xe656cae2928291e0, 0xecb11a5af1b31705 } };
    got = Fixed128_lt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge_unsigned(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0xa47a7b5ee6941cdf, 0xd09872a750a64652 } };
    const Fixed128 b = { .u64= { 0xa47a7b5ee6941cdf, 0xd09872a750a64652 } };
    got = Fixed128_lt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge_unsigned(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x44af3f13cd152d1a, 0xec3c9e4556a9f134 } };
    const Fixed128 b = { .u64= { 0xf636cf0047b3626c, 0xab191be1a0bb7fb6 } };
    got = Fixed128_lt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_le_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ge_unsigned(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x6511993d0b673bd8, 0x9e9a8da1e622e12b } };
    const Fixed128 b = { .u64= { 0x91b77af576ee7876, 0xbf1a447878e100a9 } };
    got = Fixed128_lt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_gt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge_unsigned(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const Fixed128 a = { .u64= { 0x63f40668ed6f642c, 0xcc4a988537f2555b } };
    const Fixed128 b = { .u64= { 0xc870b446c50f9b0f, 0xe0bc9aa3618ec2d9 } };
    got = Fixed128_lt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_gt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge_unsigned(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const Fixed128 a = { .u64= { 0x0dabacd08cf4ac18, 0xf37207e3e0f2f8e9 } };
    const Fixed128 b = { .u64= { 0x8ff79ca3e449bac3, 0xc8ca5618c608efb1 } };
    got = Fixed128_lt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_le_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ge_unsigned(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x86c2b78aea4cd4bc, 0x06c1b8d1708c6668 } };
    const Fixed128 b = { .u64= { 0xb207809308c03a19, 0x1ce8180998304a24 } };
    got = Fixed128_lt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_gt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge_unsigned(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const Fixed128 a = { .u64= { 0x85e26df5e10ad788, 0x02b37cf9e7b06b43 } };
    const Fixed128 b = { .u64= { 0x58cf43fed672c8b8, 0x1321cfd7d3db11b7 } };
    got = Fixed128_lt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_gt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge_unsigned(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const Fixed128 a = { .u64= { 0x8b1add60f5b9e8e7, 0x61939295742a41bb } };
    const Fixed128 b = { .u64= { 0xfa0052fec99239cc, 0x637d4bf44fa8cbd5 } };
    got = Fixed128_lt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_gt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge_unsigned(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const Fixed128 a = { .u64= { 0x7c9539a1c23aed93, 0xf75e08f2db006300 } };
    const Fixed128 b = { .u64= { 0x5f215ed42ad93787, 0x29ae9cd7dfd14872 } };
    got = Fixed128_lt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_le_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ge_unsigned(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x1ef6c4034fd33f82, 0x1c5fec718b81be4a } };
    const Fixed128 b = { .u64= { 0x5b56a0604b1f9ac5, 0xc4447b043aad7091 } };
    got = Fixed128_lt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_gt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge_unsigned(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const Fixed128 a = { .u64= { 0x70d5189ec96ef603, 0x9e11d1449ced8925 } };
    const Fixed128 b = { .u64= { 0xfc63adaf5cb7cc8b, 0x570d1f72a97e833b } };
    got = Fixed128_lt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_le_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ge_unsigned(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0xacca9dfb59b67dfd, 0xe21be37c9eeeb766 } };
    const Fixed128 b = { .u64= { 0x5463ca582dff38da, 0x5f346b34a77d1908 } };
    got = Fixed128_lt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_le_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ge_unsigned(&a, &b);
    CHECK_BOOL(got, true);
  }
  {
    const Fixed128 a = { .u64= { 0x1de83369fc834305, 0xa6cc60e92c09926a } };
    const Fixed128 b = { .u64= { 0xe59993c43cc873db, 0xb41e70d17d65dfc7 } };
    got = Fixed128_lt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_gt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge_unsigned(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const Fixed128 a = { .u64= { 0x5e1c29694f46f6b4, 0xbee46e993af96fde } };
    const Fixed128 b = { .u64= { 0xa075c01d8987af02, 0xff174aee879e66f0 } };
    got = Fixed128_lt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_gt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_le_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_ge_unsigned(&a, &b);
    CHECK_BOOL(got, false);
  }
  {
    const Fixed128 a = { .u64= { 0xcd879bb1e62607b8, 0x819be1a066749dbc } };
    const Fixed128 b = { .u64= { 0xd1471c0ef831ff82, 0x48a53be350290c45 } };
    got = Fixed128_lt_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_gt_unsigned(&a, &b);
    CHECK_BOOL(got, true);
    got = Fixed128_le_unsigned(&a, &b);
    CHECK_BOOL(got, false);
    got = Fixed128_ge_unsigned(&a, &b);
    CHECK_BOOL(got, true);
  }
}

static void fixed128_test_2str(void) {
  char buffer[1024];
  {
    const Fixed128 a = { .u64= { 0x0000000000000000, 0x0000000000000000 } };
    Fixed128_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "0");
    Fixed128_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "0");
  }
  {
    const Fixed128 a = { .u64= { 0x0000000000000000, 0x0000000100000000 } };
    Fixed128_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "1");
    Fixed128_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "1");
  }
  {
    const Fixed128 a = { .u64= { 0x0000000000000000, 0xffffffff00000000 } };
    Fixed128_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "-1");
    Fixed128_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "-1");
  }
  {
    const Fixed128 a = { .u64= { 0xffffffffffffffff, 0x7fffffffffffffff } };
    Fixed128_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "2147483647.999999999999999999999999999987378225516463811113412342955475420325228697038255631923675537109375");
    Fixed128_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "7fffffff.ffffffffffffffffffffffff");
  }
  {
    const Fixed128 a = { .u64= { 0x0000000000000000, 0x8000000000000000 } };
    Fixed128_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "-2147483648");
    Fixed128_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "-80000000");
  }
  {
    const Fixed128 a = { .u64= { 0x1027c4d1c386bbc4, 0x00003c5f414c343c } };
    Fixed128_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "15455.2550690313948101420242451449091159893631872171440252462792930288060233579017221927642822265625");
    Fixed128_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "3c5f.414c343c1027c4d1c386bbc4");
  }
  {
    const Fixed128 a = { .u64= { 0xc2ce6f447ed4d57b, 0x0000f1ca7311d8a3 } };
    Fixed128_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "61898.449491062145904275265122136842098751818868206037158106840034843543207898619584739208221435546875");
    Fixed128_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "f1ca.7311d8a3c2ce6f447ed4d57b");
  }
  {
    const Fixed128 a = { .u64= { 0x612e7696a6cecc1b, 0x00006b7fc9e9c616 } };
    Fixed128_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "27519.788723354781888244375881239822767454578413816294963560420139270235040385159663856029510498046875");
    Fixed128_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "6b7f.c9e9c616612e7696a6cecc1b");
  }
  {
    const Fixed128 a = { .u64= { 0x9755d4c13a902931, 0x00003457f1fd42a2 } };
    Fixed128_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "13399.945270695395570580904507535306622956837844640372122057871140388130015708156861364841461181640625");
    Fixed128_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "3457.f1fd42a29755d4c13a902931");
  }
  {
    const Fixed128 a = { .u64= { 0x51431193e6c3f339, 0x00000b6d07d4bedc } };
    Fixed128_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "2925.030589989460406274670300361403965747942475719432886918126845987586648334399797022342681884765625");
    Fixed128_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "b6d.07d4bedc51431193e6c3f339");
  }
  {
    const Fixed128 a = { .u64= { 0xa648a7dd06839eb9, 0x000004b68a9a021e } };
    Fixed128_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "1206.541412479861068105848895924669100705884762381206207573249022946271225009695626795291900634765625");
    Fixed128_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "4b6.8a9a021ea648a7dd06839eb9");
  }
  {
    const Fixed128 a = { .u64= { 0xed2f89d94a2f20aa, 0x0000d51505805975 } };
    Fixed128_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "54549.02148970727028945216769153256536533636856974646074159608633014073575395741499960422515869140625");
    Fixed128_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "d515.05805975ed2f89d94a2f20aa");
  }
  {
    const Fixed128 a = { .u64= { 0xb94067edfe175330, 0x000097c0dc2574bd } };
    Fixed128_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "38848.85994653350758343171104065429533051729625240720378091807685194680743734352290630340576171875");
    Fixed128_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "97c0.dc2574bdb94067edfe17533");
  }
  {
    const Fixed128 a = { .u64= { 0xe901e35cd47d380d, 0x00006133ab99254a } };
    Fixed128_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "24883.670305567539805793356407311203526416363733334598189292537206984690101307933218777179718017578125");
    Fixed128_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "6133.ab99254ae901e35cd47d380d");
  }
  {
    const Fixed128 a = { .u64= { 0x1ba1621582283d15, 0x000053d0c74803e3 } };
    Fixed128_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "21456.778442614504120140639616007095578465561620199984285977008735546434081697952933609485626220703125");
    Fixed128_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "53d0.c74803e31ba1621582283d15");
  }
  {
    const Fixed128 a = { .u64= { 0xd707107e855c3844, 0x0000bdb564ac5db9 } };
    Fixed128_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "48565.3932550982199496460742562619210749324355261131755758831352043358720038668252527713775634765625");
    Fixed128_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "bdb5.64ac5db9d707107e855c3844");
  }
  {
    const Fixed128 a = { .u64= { 0xbb968a437d5c8dfc, 0x0000f04a07923986 } };
    Fixed128_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "61514.0295749620377937819608540242827984242931448307634112980524232483503510593436658382415771484375");
    Fixed128_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "f04a.07923986bb968a437d5c8dfc");
  }
  {
    const Fixed128 a = { .u64= { 0xa5ac06d864c2f2e3, 0x000056512b9c014e } };
    Fixed128_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "22097.170349199009862767935190291363027438877804438644073914237153655193424128810875117778778076171875");
    Fixed128_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "5651.2b9c014ea5ac06d864c2f2e3");
  }
  {
    const Fixed128 a = { .u64= { 0x3a1890c78092b4d4, 0x0000064cfb695ffb } };
    Fixed128_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "1612.9820766437861456862782552537714234261962407907390931911806308818313482333905994892120361328125");
    Fixed128_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "64c.fb695ffb3a1890c78092b4d4");
  }
  {
    const Fixed128 a = { .u64= { 0x8c5fe8f8dc3bf364, 0x0000cf143b6fe507 } };
    Fixed128_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "53012.2321761268070755692154231572070181304557840996951310376383847966508255922235548496246337890625");
    Fixed128_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "cf14.3b6fe5078c5fe8f8dc3bf364");
  }
  {
    const Fixed128 a = { .u64= { 0xbab9f87ff5059285, 0x0000c47201762741 } };
    Fixed128_2str(buffer, sizeof buffer, &a, 10);
    CHECK_STR(buffer, "50290.005709126994339792924189134451134288643224189918615120616085523153060421464033424854278564453125");
    Fixed128_2str(buffer, sizeof buffer, &a, 16);
    CHECK_STR(buffer, "c472.01762741bab9f87ff5059285");
  }
}

static void fixed128_test_str2(void) {
  Fixed128 got;
  {
    const Fixed128 expect = { .u64= { 0x7b297d0b0e5e18ba, 0x0000babedeb8fc4c } };
    Fixed128_str2(&got, "47806.87001015536512756103332241253957775236318664127950863775018408574624118045903742313385009765625", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x6a17b9af5b569643, 0x000000cf58989008 } };
    Fixed128_str2(&got, "207.346077920965483533180354913517546798409637130414346034394668105704795380006544291973114013671875", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x8cfe5cd12d5db79b, 0x00005c8f959f3a51 } };
    Fixed128_str2(&got, "23695.584460873517849544238312885068798979163892197704902801806692291819445017608813941478729248046875", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x47fc816ac16e2284, 0x0000898b3fe31d03 } };
    Fixed128_str2(&got, "35211.2495592244158491209845286811814104842902604960609030428086985153868226916529238224029541015625", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0xcc1b0c3e1c07724e, 0x00005e859ff3078f } };
    Fixed128_str2(&got, "24197.62480208645511598256915272636548829029713849988848860077250702005358107271604239940643310546875", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x4a5012dc582c18c9, 0x000055be11cbc288 } };
    Fixed128_str2(&got, "21950.069515379213328549478095899802537748445453302400016287222739297391171930939890444278717041015625", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x7f1a355e526eb523, 0x00003a7779490eab } };
    Fixed128_str2(&got, "14967.473771015996228358597227017626600643635044756683488242736401740984319985727779567241668701171875", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x4fdf8e1a060cea63, 0x0000afca62f5680c } };
    Fixed128_str2(&got, "45002.386557105069981173118482156194115828114391814286488857459189727450166174094192683696746826171875", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0xcbd3f5e06bc15385, 0x0000844f3023580c } };
    Fixed128_str2(&got, "33871.188039305805275917491462178310837174943832212656999393514400953364429369685240089893341064453125", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x39b21c95055455e8, 0x0000cb6c0492c4f5 } };
    Fixed128_str2(&got, "52076.017864522809482649452935836513216480463642324809912277638890287789763533510267734527587890625", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0xb1eedaffcc3d5506, 0x0000e6d5843fdda7 } };
    Fixed128_str2(&got, "59093.51659951538197949881828506047053120056157563625892962209940062479063271894119679927825927734375", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x861e02ec39235bc0, 0x00000fb7a6048457 } };
    Fixed128_str2(&got, "4023.648506423347605261458354831990163939992220134395616015776653284774511121213436126708984375", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0xa8ea37f7523d2a54, 0x0000da43a185cc8e } };
    Fixed128_str2(&got, "55875.6309478615084253001213195960714242743690908516907456422828648356926350970752537250518798828125", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0xbcc99ae80f0c8a89, 0x000040594c717095 } };
    Fixed128_str2(&got, "16473.298605953747744497318578468055055894830873109658535996015216174015449723810888826847076416015625", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x364e433ff7c882f4, 0x0000184ae023033d } };
    Fixed128_str2(&got, "6218.8755342506831818479000970378469248572939932476723247541106065483518250402994453907012939453125", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x121b28004e6f5a94, 0x00002723dbc799b0 } };
    Fixed128_str2(&got, "10019.8585144095287488008760415740789924836378480096078205825727014399717518244870007038116455078125", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0xeacc110e4f73fd94, 0x00009883f07534fe } };
    Fixed128_str2(&got, "39043.9392884371143710588988853514686540343193234605206312810843360949775160406716167926788330078125", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x9f7a7dafb43adc4f, 0x0000132982458cc8 } };
    Fixed128_str2(&got, "4905.508873747806754618360545189204561494051150124660721856505174454099460490397177636623382568359375", NULL);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x334de73d60c290d0, 0x000032b358d07674 } };
    Fixed128_str2(&got, "12979.34693088852809915778260389473788852114984942990216949809667568160875816829502582550048828125", NULL);
    CHECK(got, expect);
  }
}

static void fixed128_test_double2(void) {
  Fixed128 got;
  {
    const Fixed128 expect = { .u64= { 0x0000000000000000, 0x0000000000000000 } };
    Fixed128_double2(&got, -9.229944107861122e-30);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x00000000000000b1, 0x0000000000000000 } };
    Fixed128_double2(&got, 2.245237901104122e-27);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x000000000000870d, 0x0000000000000000 } };
    Fixed128_double2(&got, 4.363785018020341e-25);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0xffffffffff829869, 0xffffffffffffffff } };
    Fixed128_double2(&got, -1.0373230250090241e-22);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0xfffffffffda9aaf1, 0xffffffffffffffff } };
    Fixed128_double2(&got, -4.949288492124743e-22);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0xffffffe623b15e3a, 0xffffffffffffffff } };
    Fixed128_double2(&got, -1.4019045705210807e-18);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x00004d9d98184b9d, 0x0000000000000000 } };
    Fixed128_double2(&got, 1.0771328556043823e-15);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x0093d38c0d6fe640, 0x0000000000000000 } };
    Fixed128_double2(&got, 5.25184716271748e-13);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x300e5d1f390b2000, 0xffffffffffffffff } };
    Fixed128_double2(&got, -1.8912386765040695e-10);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x838f792c1b300000, 0xffffffffffffff0e } };
    Fixed128_double2(&got, -5.62253622386448e-08);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x6758f2a0a0000000, 0x000000000000abe9 } };
    Fixed128_double2(&got, 1.0246737790523367e-05);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x26e0439000000000, 0xffffffffffdd93a5 } };
    Fixed128_double2(&got, -0.0005252572819917705);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x8c8e280000000000, 0xffffffffc324c998 } };
    Fixed128_double2(&got, -0.237719917542058);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x4250000000000000, 0xffffff01138166d2 } };
    Fixed128_double2(&got, -254.92380673758242);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0xc800000000000000, 0xffffe409584c2666 } };
    Fixed128_double2(&got, -7158.655088043124);
    CHECK(got, expect);
  }
  {
    const Fixed128 expect = { .u64= { 0x0000000000000000, 0x00716db13a9a4680 } };
    Fixed128_double2(&got, 7433649.228916556);
    CHECK(got, expect);
  }
}

static void fixed128_test_2double(void) {
  {
    const Fixed128 a = { .u64= { 0x1027c4d1c386bbc4, 0x00003c5f414c343c } };
    double got = Fixed128_2double(&a);
    CHECK_DOUBLE(got, 15455.255069031394668854773044586181640625);
  }
  {
    const Fixed128 a = { .u64= { 0xc2ce6f447ed4d57b, 0x0000f1ca7311d8a3 } };
    double got = Fixed128_2double(&a);
    CHECK_DOUBLE(got, 61898.4494910621433518826961517333984375);
  }
  {
    const Fixed128 a = { .u64= { 0x612e7696a6cecc1b, 0x00006b7fc9e9c616 } };
    double got = Fixed128_2double(&a);
    CHECK_DOUBLE(got, 27519.78872335478081367909908294677734375);
  }
  {
    const Fixed128 a = { .u64= { 0x9755d4c13a902931, 0x00003457f1fd42a2 } };
    double got = Fixed128_2double(&a);
    CHECK_DOUBLE(got, 13399.9452706953961751423776149749755859375);
  }
  {
    const Fixed128 a = { .u64= { 0x51431193e6c3f339, 0x00000b6d07d4bedc } };
    double got = Fixed128_2double(&a);
    CHECK_DOUBLE(got, 2925.03058998946062274626456201076507568359375);
  }
  {
    const Fixed128 a = { .u64= { 0xa648a7dd06839eb9, 0x000004b68a9a021e } };
    double got = Fixed128_2double(&a);
    CHECK_DOUBLE(got, 1206.541412479861037354567088186740875244140625);
  }
  {
    const Fixed128 a = { .u64= { 0xed2f89d94a2f20aa, 0x0000d51505805975 } };
    double got = Fixed128_2double(&a);
    CHECK_DOUBLE(got, 54549.021489707272849045693874359130859375);
  }
  {
    const Fixed128 a = { .u64= { 0xb94067edfe175330, 0x000097c0dc2574bd } };
    double got = Fixed128_2double(&a);
    CHECK_DOUBLE(got, 38848.8599465335064451210200786590576171875);
  }
  {
    const Fixed128 a = { .u64= { 0xe901e35cd47d380d, 0x00006133ab99254a } };
    double got = Fixed128_2double(&a);
    CHECK_DOUBLE(got, 24883.6703055675388895906507968902587890625);
  }
  {
    const Fixed128 a = { .u64= { 0x1ba1621582283d15, 0x000053d0c74803e3 } };
    double got = Fixed128_2double(&a);
    CHECK_DOUBLE(got, 21456.77844261450445628724992275238037109375);
  }
  {
    const Fixed128 a = { .u64= { 0xd707107e855c3844, 0x0000bdb564ac5db9 } };
    double got = Fixed128_2double(&a);
    CHECK_DOUBLE(got, 48565.3932550982208340428769588470458984375);
  }
  {
    const Fixed128 a = { .u64= { 0xbb968a437d5c8dfc, 0x0000f04a07923986 } };
    double got = Fixed128_2double(&a);
    CHECK_DOUBLE(got, 61514.0295749620345304720103740692138671875);
  }
  {
    const Fixed128 a = { .u64= { 0xa5ac06d864c2f2e3, 0x000056512b9c014e } };
    double got = Fixed128_2double(&a);
    CHECK_DOUBLE(got, 22097.17034919900834211148321628570556640625);
  }
  {
    const Fixed128 a = { .u64= { 0x3a1890c78092b4d4, 0x0000064cfb695ffb } };
    double got = Fixed128_2double(&a);
    CHECK_DOUBLE(got, 1612.982076643786058411933481693267822265625);
  }
  {
    const Fixed128 a = { .u64= { 0x8c5fe8f8dc3bf364, 0x0000cf143b6fe507 } };
    double got = Fixed128_2double(&a);
    CHECK_DOUBLE(got, 53012.232176126810372807085514068603515625);
  }
  {
    const Fixed128 a = { .u64= { 0xbab9f87ff5059285, 0x0000c47201762741 } };
    double got = Fixed128_2double(&a);
    CHECK_DOUBLE(got, 50290.0057091269918601028621196746826171875);
  }
}


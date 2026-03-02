import { Injectable, NotFoundException, BadRequestException, UnauthorizedException } from '@nestjs/common';
import { PrismaService } from '../prisma/prisma.service';
import * as bcrypt from 'bcrypt';

@Injectable()
export class UsersService {
  constructor(private prisma: PrismaService) {}

  findAll() {
    return this.prisma.user.findMany({ where: { isDeleted: false }, orderBy: { createdAt: 'desc' } });
  }
  async findOne(id: string) {
    const user = await this.prisma.user.findFirst({ where: { id, isDeleted: false } });
    if (!user) throw new NotFoundException('User not found');
    return user;
  }
  async create(data: any) {
    return this.prisma.user.create({ data });
  }
  async update(id: string, data: any) {
    await this.findOne(id);
    return this.prisma.user.update({ where: { id }, data });
  }
  async softDelete(id: string) {
    await this.findOne(id);
    return this.prisma.user.update({ where: { id }, data: { isDeleted: true } });
  }
  async changePassword(id: string, currentPassword: string, newPassword: string) {
    const user = await this.findOne(id);
    if (!user.passwordHash) throw new BadRequestException('Password not set');
    const ok = await bcrypt.compare(currentPassword, user.passwordHash);
    if (!ok) throw new UnauthorizedException('Invalid current password');
    const passwordHash = await bcrypt.hash(newPassword, 12);
    return this.prisma.user.update({ where: { id }, data: { passwordHash } });
  }
}


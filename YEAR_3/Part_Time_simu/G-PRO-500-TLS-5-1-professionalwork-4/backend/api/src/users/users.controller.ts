import { Body, Controller, Get, Param, Patch, Post, Delete } from '@nestjs/common';
import { UsersService } from './users.service';
import { CreateUserDto } from './dto/create-user.dto';
import { UpdateUserDto } from './dto/update-user.dto';
import { UpdatePasswordDto } from './dto/update-password.dto';

@Controller('users')
export class UsersController {
  constructor(private readonly users: UsersService) {}

  @Get() findAll() { return this.users.findAll(); }
  @Get(':id') findOne(@Param('id') id: string) { return this.users.findOne(id); }
  @Post() create(@Body() dto: CreateUserDto) { return this.users.create(dto); }
  @Patch(':id') update(@Param('id') id: string, @Body() dto: UpdateUserDto) {
    return this.users.update(id, dto);
  }
  @Patch(':id/password') updatePassword(@Param('id') id: string, @Body() dto: UpdatePasswordDto) {
    return this.users.changePassword(id, dto.currentPassword, dto.newPassword);
  }
  @Delete(':id') remove(@Param('id') id: string) { return this.users.softDelete(id); }
}
